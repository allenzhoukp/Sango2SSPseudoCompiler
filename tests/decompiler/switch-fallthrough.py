#!/usr/bin/env python3
"""Regression test: switch case bodies must not silently fall through.

The historical defect: in the structured switch path, a case body whose
control flow terminated at the default target (or a non-adjacent case) was
emitted with no terminator, so the generated C fell into the next case body.
The generated source compiled and round-tripped, but case 1 executed case 2's
body and never reached default.

The fixture switch: case 1 jumps to the default body; case 2 breaks; the
default body breaks.  A case-1 hit must end with v1 == 33 and never execute
v1 = 22.
"""

import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fixture_objects  # noqa: E402


def switch_blocks(source):
    """Extracts the case/default regions of each switch in the source."""
    switches = []
    depth = 0
    cases = None
    current = None
    for line in source.splitlines():
        text = line.strip()
        if depth == 0 and text.startswith("switch ("):
            depth = 1
            cases = []
            current = None
            continue
        if depth == 0:
            continue
        match = re.match(r"case (-?\d+):|default:", text)
        if match:
            if current is not None:
                cases.append(current)
            current = [text, []]
            continue
        if text == "}":
            if current is not None:
                cases.append(current)
            switches.append(cases)
            depth = 0
            continue
        if current is not None:
            current[1].append(text)
    return switches


def case_executes(region, body_text):
    """Returns True when a region's statements include body_text."""
    return any(body_text in statement for statement in region[1])


def main():
    repository_root = os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "..", "..")
    decompiler = os.path.join(repository_root, "sg2decomp.exe")
    if not os.path.exists(decompiler):
        raise SystemExit("build sg2decomp.exe first (make decomp)")

    with tempfile.TemporaryDirectory(prefix="sango-switch-fall-") as work:
        object_path = os.path.join(work, "switchfall.so")
        source_path = os.path.join(work, "switchfall.cpp")
        report_path = os.path.join(work, "switchfall.json")
        with open(object_path, "wb") as output:
            output.write(fixture_objects.switch_fallthrough())

        result = subprocess.run(
            [decompiler, object_path, "-o", source_path,
             "--report", report_path, "--mode", "structured"],
            capture_output=True, text=True)
        if result.returncode != 0:
            raise SystemExit("decompiler failed: " + result.stderr)

        with open(source_path, encoding="utf-8") as source_file:
            source = source_file.read()

        problems = []
        for region in switch_blocks(source):
            # Every non-empty case body must be terminated by break, return,
            # or a goto/dispatcher transfer: a body that simply ends while a
            # following case exists is an implicit wrong fallthrough.
            for index, (label, body) in enumerate(region):
                statements = [s for s in body if s.strip()]
                if not statements:
                    continue
                terminated = any("break;" in s or s.startswith("return")
                                 or "goto " in s or "control_state_" in s
                                 for s in body)
                if not terminated and index + 1 < len(region):
                    problems.append(
                        "%s body '%s' falls through into %s"
                        % (label, statements[0], region[index + 1][0]))


        # The dispatcher form is the correct conservative fallback for this
        # non-adjacent transfer.  Verify that state 3 (case 1) goes directly to
        # state 5 (default), rather than falling through state 4 (case 2).
        if "control_state_0" in source:
            ordering = [
                source.find("case 3:"),
                source.find("v1 = 11"),
                source.find("control_state_0 = 5", source.find("v1 = 11")),
                source.find("case 5:"),
            ]
            eleven = source.find("v1 = 11")
            thirtythree = source.find("v1 = 33")
            if min(ordering + [eleven, thirtythree]) < 0:
                problems.append("dispatcher lost the case-1 to default path")
            elif ordering != sorted(ordering):
                problems.append("case 1 does not transfer directly to default")

        if problems:
            print("FAILED switch fallthrough:")
            for problem in problems:
                print("  -", problem)
            print(source)
            return 1
        print("PASS switch fallthrough")
        return 0


if __name__ == "__main__":
    sys.exit(main())
