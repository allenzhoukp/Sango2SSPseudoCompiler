#!/usr/bin/env python3
"""Regression test: float constants must survive decompilation as floats.

The historical defect: TypeAnalyzer eagerly typed every PUSH as integer, so a
later CMPLF float requirement turned the constant node into a conflict and
StackLifter rendered the raw bit pattern (e.g. 1058261487) as an integer
literal.  Recompiling that source compared against ~1.06e9 instead of
0.5773f, changing program behavior while still compiling cleanly.
"""

import os
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fixture_objects  # noqa: E402


def main():
    repository_root = os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "..", "..")
    decompiler = os.path.join(repository_root, "sg2decomp.exe")
    if not os.path.exists(decompiler):
        raise SystemExit("build sg2decomp.exe first (make decomp)")

    with tempfile.TemporaryDirectory(prefix="sango-float-const-") as work:
        object_path = os.path.join(work, "floatconst.so")
        source_path = os.path.join(work, "floatconst.cpp")
        report_path = os.path.join(work, "floatconst.json")
        with open(object_path, "wb") as output:
            output.write(fixture_objects.float_const())

        result = subprocess.run(
            [decompiler, object_path, "-o", source_path,
             "--report", report_path, "--mode", "structured"],
            capture_output=True, text=True)
        if result.returncode != 0:
            raise SystemExit("decompiler failed: " + result.stderr)

        with open(source_path, encoding="utf-8") as source_file:
            source = source_file.read()

        problems = []
        if "0.577300012" not in source:
            problems.append("float literal 0.577300012 missing")
        if "1058261487" in source:
            problems.append("raw float bit pattern emitted as integer")
        if "float v2;" not in source:
            problems.append("local slot 2 not declared as float")

        if problems:
            print("FAILED float-constant typing:")
            for problem in problems:
                print("  -", problem)
            print(source)
            return 1

        print("PASS float-constant typing")
        return 0


if __name__ == "__main__":
    sys.exit(main())
