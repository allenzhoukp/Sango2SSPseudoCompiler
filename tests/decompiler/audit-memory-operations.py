#!/usr/bin/env python3

"""Audit memory-operation emission across every object fixture."""

import argparse
from dataclasses import dataclass
import json
from pathlib import Path
import re
import subprocess
import tempfile


MEMORY_FUNCTIONS = (
    "GetByte", "GetByte_Old", "GetByte_Signed_Old",
    "GetShort", "GetShort_Old", "GetShort_Signed_Old",
    "GetInt", "GetInt_Old", "GetString",
    "SetByte", "SetByte_Old", "SetShort", "SetShort_Old",
    "SetInt", "SetInt_Old", "SetString", "SetString_Old",
)
MEMORY_NAME_PATTERN = "(?:" + "|".join(
    sorted(MEMORY_FUNCTIONS, key=len, reverse=True)
) + ")"
HIGH_LEVEL_MEMORY_CALL = re.compile(r"\b" + MEMORY_NAME_PATTERN + r"\s*\(")
FUNCTION_DECLARATION = re.compile(
    r"^\s*(?:extern\s+)?(?:void|int|float|string)\s+"
    + MEMORY_NAME_PATTERN + r"\s*\("
)
ASSEMBLY_MEMORY_OPERATION = re.compile(
    r"\b(?:CALL\s+" + MEMORY_NAME_PATTERN
    + r"|SYSCALL\s+(?:0x200|0x21[3-9]))\b",
    re.IGNORECASE,
)


@dataclass(frozen=True)
class ObjectAudit:
    """Stores decompilation coverage and memory-operation counts for one object."""

    name: str
    functions: int
    high_level_functions: int
    naked_assembly_functions: int
    inline_assembly_blocks: int
    dispatcher_functions: int
    residual_gotos: int
    automatically_included_functions: int
    input_memory_operations: int
    high_level_memory_calls: int
    assembly_memory_operations: int

    def preserves_memory_operations(self) -> bool:
        """Returns whether output represents at least every input operation."""

        return self.input_memory_operations <= (
            self.high_level_memory_calls + self.assembly_memory_operations
        )

    def memory_operation_delta(self) -> int:
        """Returns represented output operations minus input operations."""

        return (
            self.high_level_memory_calls + self.assembly_memory_operations
            - self.input_memory_operations
        )


def find_objects(repository: Path) -> list[Path]:
    """Finds object fixtures at the repository root and one directory below it."""

    objects = []
    for path in repository.glob("*"):
        if path.is_file() and path.suffix.lower() == ".so":
            objects.append(path)
        elif path.is_dir():
            objects.extend(
                child for child in path.iterdir()
                if child.is_file() and child.suffix.lower() == ".so"
            )
    return sorted(
        (path for path in objects if not path.name.startswith("_decompiler_")),
        key=lambda path: str(path).lower(),
    )


def count_high_level_memory_calls(source: str) -> int:
    """Counts named memory calls while excluding declarations and definitions."""

    return sum(
        len(HIGH_LEVEL_MEMORY_CALL.findall(line))
        for line in source.splitlines()
        if not FUNCTION_DECLARATION.search(line)
    )


def count_assembly_memory_operations(source: str) -> int:
    """Counts helper calls and configured memory system calls in assembly text."""

    return len(ASSEMBLY_MEMORY_OPERATION.findall(source))


def run_decompiler(decompiler: Path, object_path: Path, output_directory: Path,
                   mode: str) -> tuple[str, dict]:
    """Runs one decompilation mode and returns its source and parsed report."""

    stem = object_path.name.replace(".", "_") + "-" + mode
    source_path = output_directory / (stem + ".cpp")
    report_path = output_directory / (stem + ".json")
    subprocess.run(
        [str(decompiler), str(object_path), "-o", str(source_path),
         "--report", str(report_path), "--mode", mode],
        check=True,
        cwd=decompiler.parent,
        stdout=subprocess.DEVNULL,
    )
    source = source_path.read_text(encoding="utf-8", errors="replace")
    with report_path.open(encoding="utf-8") as report_file:
        report = json.load(report_file)
    return source, report


def audit_object(decompiler: Path, object_path: Path,
                 output_directory: Path) -> ObjectAudit:
    """Audits one object's structured and lossless memory-operation emission."""

    structured_source, report = run_decompiler(
        decompiler, object_path, output_directory, "structured"
    )
    lossless_source, _ = run_decompiler(
        decompiler, object_path, output_directory, "lossless"
    )
    summary = report["summary"]
    return ObjectAudit(
        name=object_path.name,
        functions=len(report["functions"]),
        high_level_functions=summary["high_level_functions"],
        naked_assembly_functions=summary["naked_assembly_functions"],
        inline_assembly_blocks=summary["inline_assembly_blocks"],
        dispatcher_functions=summary["dispatcher_functions"],
        residual_gotos=summary["residual_gotos"],
        automatically_included_functions=summary["automatically_included_functions"],
        input_memory_operations=count_assembly_memory_operations(lossless_source),
        high_level_memory_calls=count_high_level_memory_calls(structured_source),
        assembly_memory_operations=count_assembly_memory_operations(structured_source),
    )


def print_audits(audits: list[ObjectAudit]) -> None:
    """Prints a compact table of coverage and memory-emission results."""

    print(
        "object\thigh/total\tnaked\tinline\tdispatch\tgoto\t"
        "memory input/high/assembly/delta\tsg2lang"
    )
    for audit in audits:
        print(
            f"{audit.name}\t{audit.high_level_functions}/{audit.functions}\t"
            f"{audit.naked_assembly_functions}\t{audit.inline_assembly_blocks}\t"
            f"{audit.dispatcher_functions}\t{audit.residual_gotos}\t"
            f"{audit.input_memory_operations}/"
            f"{audit.high_level_memory_calls}/"
            f"{audit.assembly_memory_operations}/"
            f"{audit.memory_operation_delta():+d}\t"
            f"{audit.automatically_included_functions}"
        )


def main() -> int:
    """Runs the non-mutating corpus audit and rejects lost memory operations."""

    repository = Path(__file__).resolve().parents[2]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "objects", nargs="*", type=Path,
        help="specific objects to audit; defaults to the complete fixture corpus",
    )
    parser.add_argument(
        "--decompiler", type=Path, default=repository / "sg2decomp.exe",
    )
    parser.add_argument(
        "--require-exact-counts", action="store_true",
        help="also fail when structured output contains additional occurrences",
    )
    arguments = parser.parse_args()
    decompiler = arguments.decompiler.resolve()
    objects = [path.resolve() for path in arguments.objects]
    if not objects:
        objects = find_objects(repository)

    with tempfile.TemporaryDirectory(prefix="sango-memory-audit.") as directory:
        output_directory = Path(directory)
        audits = [
            audit_object(decompiler, object_path, output_directory)
            for object_path in objects
        ]
    print_audits(audits)
    failures = [audit.name for audit in audits if not audit.preserves_memory_operations()]
    if failures:
        print("Lost memory-operation occurrences: " + ", ".join(failures))
        return 1
    expansions = [
        audit.name for audit in audits if audit.memory_operation_delta() > 0
    ]
    if expansions:
        print("Expanded memory-operation counts: " + ", ".join(expansions))
        if arguments.require_exact_counts:
            return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
