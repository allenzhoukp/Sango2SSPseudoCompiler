#!/usr/bin/env python3

"""Compare broad control structures in two Sango source files."""

import argparse
from dataclasses import dataclass
import re
from pathlib import Path


FUNCTION_HEADER = re.compile(
    r"^(?:void|int|float|string)[ \t]+(?P<name>[A-Za-z_]\w*)"
    r"[ \t]*\([^\n]*\)(?:[ \t]+callsign[ \t]+\d+)?[ \t]*\{",
    re.MULTILINE,
)


@dataclass(frozen=True)
class StructureCounts:
    """Stores source-construct counts used for a coarse comparison."""

    conditions: int
    loops: int
    switches: int


def find_function_end(source: str, opening_brace: int) -> int:
    """Returns the position just after a function's balanced closing brace."""

    depth = 0
    for position in range(opening_brace, len(source)):
        if source[position] == "{":
            depth += 1
        elif source[position] == "}":
            depth -= 1
            if depth == 0:
                return position + 1
    raise ValueError("function has no balanced closing brace")


def extract_functions(path: Path) -> dict[str, str]:
    """Reads a source file and returns each complete function by name."""

    source = path.read_text(encoding="utf-8", errors="replace").replace("\r", "")
    functions: dict[str, str] = {}
    for header in FUNCTION_HEADER.finditer(source):
        name = header.group("name")
        if name in functions:
            raise ValueError(f"duplicate function name {name!r} in {path}")
        opening_brace = source.find("{", header.start(), header.end())
        end = find_function_end(source, opening_brace)
        functions[name] = source[header.start():end]
    return functions


def count_structures(function_source: str) -> StructureCounts:
    """Counts explicit condition, loop, and switch statements in one function."""

    return StructureCounts(
        conditions=len(re.findall(r"\bif[ \t]*\(", function_source)),
        loops=len(re.findall(r"\bwhile[ \t]*\(", function_source)),
        switches=len(re.findall(r"\bswitch[ \t]*\(", function_source)),
    )


def compare(reference: dict[str, str], candidate: dict[str, str]) -> tuple[list[str], list[str]]:
    """Returns names with missing bodies and differing construct counts."""

    missing = sorted(set(reference) - set(candidate))
    different = sorted(
        name for name in set(reference) & set(candidate)
        if count_structures(reference[name]) != count_structures(candidate[name])
    )
    return missing, different


def dispatcher_functions(functions: dict[str, str]) -> list[str]:
    """Returns functions that use the decompiler's local-state dispatcher."""

    return sorted(
        name for name, source in functions.items()
        if "control_state_" in source
    )


def main() -> int:
    """Prints a concise structural comparison suitable for corpus review."""

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("reference", type=Path)
    parser.add_argument("candidate", type=Path)
    parser.add_argument(
        "--details", action="store_true",
        help="list every function whose construct counts differ",
    )
    arguments = parser.parse_args()

    reference = extract_functions(arguments.reference)
    candidate = extract_functions(arguments.candidate)
    missing, different = compare(reference, candidate)
    dispatchers = dispatcher_functions(candidate)
    matching = len(set(reference) & set(candidate)) - len(different)

    print(f"Reference functions: {len(reference)}")
    print(f"Candidate functions: {len(candidate)}")
    print(f"Matching if/while/switch counts: {matching}")
    print(f"Different if/while/switch counts: {len(different)}")
    print(f"Dispatcher fallbacks: {len(dispatchers)}")
    if dispatchers:
        print("Dispatcher functions: " + ", ".join(dispatchers))
    if arguments.details and different:
        print("Different functions: " + ", ".join(different))
    if missing:
        print("Missing functions: " + ", ".join(missing))
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
