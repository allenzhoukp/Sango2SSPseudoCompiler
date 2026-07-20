#!/usr/bin/env bash
set -euo pipefail

repo_root=$(cd "$(dirname "$0")/../.." && pwd)
compiler=${1:-"$repo_root/ss.exe"}
compiler_dir=$(cd "$(dirname "$compiler")" && pwd)
compiler="$compiler_dir/$(basename "$compiler")"

test_dir=$(mktemp -d /tmp/ss-label-reduction.XXXXXX)
trap 'rm -rf -- "$test_dir"' EXIT

cp -R "$repo_root/config" "$test_dir/config"
cp "$repo_root/tests/performance/label_reduction.cpp" "$test_dir/"

# Keep this fixture independent of the optional sg2lang.h prelude.
sed -i 's/use_exe_memory_io = 0/use_exe_memory_io = 1/' \
    "$test_dir/config/CONFIG.txt"

normalize_output() {
    sed -E \
        -e 's/t[0-9]+_([0-9]+_[[:alnum:]_]+)/l\1/g' \
        -e 's/Compilation Timestamp: [^"]*/Compilation Timestamp: <normalized>/' \
        "$1"
}

run_case() {
    local timed=$1
    local name=$2
    local output="$test_dir/label-reduction-$name.asm"
    local normalized="$test_dir/label-reduction-$name.normalized.asm"

    sed -i -E "s/use_timed_label = [01]/use_timed_label = $timed/" \
        "$test_dir/config/CONFIG.txt"

    (cd "$test_dir" && "$compiler" label_reduction.cpp "$output" >/dev/null)
    normalize_output "$output" > "$normalized"
    diff -u "$repo_root/tests/performance/label_reduction.expected.asm" \
        "$normalized"
}

run_case 0 untimed
run_case 1 timed

echo "label reduction fixtures passed (timed and untimed)"
