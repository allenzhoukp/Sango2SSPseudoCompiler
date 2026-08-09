#!/usr/bin/env python3
"""Documents the current failure for calls into mid-function addresses.

The assembly fallback names the target block, but no label is emitted for it.
The assembler must therefore reject the generated source with a defined and
repeatable error instead of silently producing a wrong object.
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
    compiler = os.path.join(repository_root, "ss.exe")
    assembler = os.path.join(repository_root, "sg2asm.exe")
    for tool in (decompiler, compiler, assembler):
        if not os.path.exists(tool):
            raise SystemExit("missing tool " + tool)

    with tempfile.TemporaryDirectory(prefix="sango-midcall-") as work:
        object_path = os.path.join(work, "midcall.so")
        source_path = os.path.join(work, "midcall.cpp")
        report_path = os.path.join(work, "midcall.json")
        with open(object_path, "wb") as output:
            output.write(fixture_objects.mid_function_call())

        result = subprocess.run(
            [decompiler, object_path, "-o", source_path,
             "--report", report_path, "--mode", "structured"],
            capture_output=True, text=True)
        if result.returncode != 0:
            raise SystemExit("decompiler failed: " + result.stderr)

        with open(source_path, encoding="utf-8") as source_file:
            source = source_file.read()

        compiler_input = os.path.join(repository_root, "_audit_midcall.cpp")
        with open(compiler_input, "w", encoding="utf-8") as output:
            output.write(source)
        try:
            compile_result = subprocess.run(
                [compiler, "_audit_midcall.cpp"], cwd=repository_root,
                capture_output=True, text=True)
            if compile_result.returncode != 0:
                print("PASS mid-function call (compiler rejection documented)")
                return 0

            assemble_result = subprocess.run(
                [assembler, "_audit_midcall.asm"],
                cwd=repository_root, capture_output=True, text=True,
                stdin=subprocess.DEVNULL)
            message = (assemble_result.stdout + assemble_result.stderr).strip()
            if "Symbol not defined" in message:
                print("PASS mid-function call (undefined label documented)")
                return 0
            print("FAIL mid-function call unexpectedly assembled:", message)
            return 1
        finally:
            for suffix in (".cpp", ".asm", ".lst"):
                leftover = os.path.join(
                    repository_root, "_audit_midcall" + suffix)
                if os.path.exists(leftover):
                    os.remove(leftover)


if __name__ == "__main__":
    sys.exit(main())
