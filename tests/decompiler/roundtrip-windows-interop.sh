#!/usr/bin/env bash

set -euo pipefail

repository_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)
temporary_directory=$(mktemp -d /tmp/sango-decompiler-roundtrip.XXXXXX)
run_suffix=${temporary_directory##*.}
temporary_prefix="$repository_root/_decompiler_roundtrip_$run_suffix"
temporary_source="$temporary_prefix.cpp"
temporary_assembly="$temporary_prefix.asm"
temporary_object="$temporary_prefix.cds"
temporary_listing="$temporary_prefix.lst"
temporary_sango7="$temporary_prefix-sango7.so"
temporary_sango7_assembly="$temporary_prefix-sango7.asm"
temporary_sango7_object="$temporary_prefix-sango7.cds"
temporary_sango7_listing="$temporary_prefix-sango7.lst"

# Removes only files created by this test run.
cleanup() {
    rm -f "$temporary_source" "$temporary_assembly" "$temporary_object" \
        "$temporary_listing" "$temporary_sango7" \
        "$temporary_sango7_assembly" "$temporary_sango7_object" \
        "$temporary_sango7_listing"
    rm -rf "$temporary_directory"
}
trap cleanup EXIT

# Verifies that a decompiler report contains no residual source goto.
check_report() {
    python3 - "$1" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as report_file:
    report = json.load(report_file)
if report["summary"]["residual_gotos"] >= 10:
    raise SystemExit("too many residual gotos in " + sys.argv[1])
PY
}

# Verifies that the unchanged compiler can accept a generated source file.
check_source_preflight() {
    python3 - "$1" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as report_file:
    report = json.load(report_file)
if not report["unchanged_compiler_preflight"]["within_known_fixed_limits"]:
    raise SystemExit("unexpected compiler-limit failure in " + sys.argv[1])
PY
}

# Verifies that a generated source needs no whole-function assembly fallback.
check_no_naked_functions() {
    python3 - "$1" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as report_file:
    report = json.load(report_file)
if report["summary"]["naked_assembly_functions"] != 0:
    raise SystemExit("unexpected naked assembly in " + sys.argv[1])
PY
}

# Verifies that a primary readability target needs no assembly fallback at all.
check_no_assembly_fallback() {
    check_no_naked_functions "$1"
    python3 - "$1" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as report_file:
    report = json.load(report_file)
if report["summary"]["inline_assembly_blocks"] != 0:
    raise SystemExit("unexpected inline assembly in " + sys.argv[1])
PY
}

# Verifies that magic.so retains fully structured control flow.
check_magic_structure() {
    python3 - "$1" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as report_file:
    report = json.load(report_file)
scale_shadow = next(
    function for function in report["functions"]
    if function["name"] == "ScaleShadow"
)
if "dispatcher" in scale_shadow["reason"].lower():
    raise SystemExit("ScaleShadow unexpectedly uses a dispatcher")
if report["summary"]["dispatcher_functions"] != 0:
    raise SystemExit("magic.so unexpectedly uses a dispatcher")
PY
}

# Verifies that every original function interface survives regeneration.
compare_interfaces() {
    python3 - "$1" "$2" <<'PY'
from collections import Counter
import json
import sys

def interfaces(path):
    with open(path, encoding="utf-8") as report_file:
        report = json.load(report_file)
    return Counter(
        (function["name"], function["parameter_count"], function["call_sign"])
        for function in report["functions"]
    )

missing = interfaces(sys.argv[1]) - interfaces(sys.argv[2])
if missing:
    raise SystemExit("regenerated object lost interfaces: " + repr(missing))
PY
}

# Runs the source compile, Windows assembly, and re-decode loop for one object.
roundtrip_source_object() {
    local object_path=$1
    local result_number=$2
    local original_report="$temporary_directory/$result_number.original.json"
    local regenerated_report="$temporary_directory/$result_number.regenerated.json"

    "$repository_root/sg2decomp.exe" "$object_path" \
        -o "$temporary_source" --report "$original_report" --mode structured
    check_report "$original_report"
    check_source_preflight "$original_report"
    case "${object_path,,}" in
        "./magic.so"|"./system.so")
            check_no_assembly_fallback "$original_report"
            if [[ ${object_path,,} == "./magic.so" ]]; then
                check_magic_structure "$original_report"
                python3 tests/decompiler/compare-structure.py \
                    magic_structured.cpp "$temporary_source"
            fi
            ;;
        "./system_somenew.so")
            check_no_naked_functions "$original_report"
            ;;
    esac

    (
        cd "$repository_root"
        ./ss.exe "$(basename "$temporary_source")"
        ./sg2asm.exe "$(basename "$temporary_assembly")" </dev/null
    )
    cp "$temporary_object" "$temporary_directory/$result_number.so"
    "$repository_root/sg2decomp.exe" "$temporary_directory/$result_number.so" \
        -o "$temporary_directory/$result_number.cpp" \
        --report "$regenerated_report" --mode structured
    check_report "$regenerated_report"
    compare_interfaces "$original_report" "$regenerated_report"
}

# Verifies byte-identical assembly preservation for an object too large for ss.exe.
roundtrip_large_object() {
    local object_path=$1
    cp "$object_path" "$temporary_sango7"
    /tmp/sango-current-disassembler "$temporary_sango7"
    (
        cd "$repository_root"
        ./sg2asm.exe "$(basename "$temporary_sango7_assembly")" </dev/null
    )
    cmp "$temporary_sango7" "$temporary_sango7_object"
}

cd "$repository_root"
make decomp cc-release
g++ -std=c++11 -O2 cc/Sg2dasm2.cpp -o /tmp/sango-current-disassembler

mapfile -d '' objects < <(find . -maxdepth 2 -type f -iname '*.so' \
    ! -name '_decompiler_*' -print0 | sort -z)
result_number=0
for object_path in "${objects[@]}"; do
    if [[ $object_path == "./Sango7Script.so" ]]; then
        ./sg2decomp.exe "$object_path" \
            -o "$temporary_directory/sango7.cpp" \
            --report "$temporary_directory/sango7.json" --mode structured
        check_report "$temporary_directory/sango7.json"
        roundtrip_large_object "$object_path"
    else
        roundtrip_source_object "$object_path" "$result_number"
        result_number=$((result_number + 1))
    fi
done

printf 'Validated %d source round trips and the exact Sango7 assembly path.\n' \
    "$result_number"
