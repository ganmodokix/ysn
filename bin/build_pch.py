#!/usr/bin/env python3

import hashlib
import json
from pathlib import Path
import shutil
import subprocess

from run import COMPILER, COMPILE_FLAGS, PCH_BUILD_FLAGS, PCH_CONTENT, PCH_GCH, PCH_HEADER


def compiler_identity() -> dict[str, str | int]:
    compiler_path = shutil.which(COMPILER)
    if compiler_path is None:
        raise FileNotFoundError(f"Compiler not found: {COMPILER}")

    resolved_compiler = Path(compiler_path).resolve()
    cc1plus = subprocess.check_output(
        [COMPILER, "-print-prog-name=cc1plus"], text=True
    ).strip()
    cc1plus_path = Path(cc1plus).resolve()

    return {
        "path": str(resolved_compiler),
        "mtime_ns": resolved_compiler.stat().st_mtime_ns,
        "cc1plus_path": str(cc1plus_path),
        "cc1plus_mtime_ns": cc1plus_path.stat().st_mtime_ns,
        "version": subprocess.check_output(
            [COMPILER, "-dumpfullversion", "-dumpversion"], text=True
        ).strip(),
    }


def pch_signature() -> str:
    payload = {
        "compiler": compiler_identity(),
        "compile_flags": COMPILE_FLAGS,
        "pch_build_flags": PCH_BUILD_FLAGS,
        "content": PCH_CONTENT,
    }
    encoded = json.dumps(payload, sort_keys=True).encode()
    return hashlib.sha256(encoded).hexdigest()


def main() -> int:
    PCH_HEADER.parent.mkdir(parents=True, exist_ok=True)
    if not PCH_HEADER.exists() or PCH_HEADER.read_text(encoding="utf8") != PCH_CONTENT:
        PCH_HEADER.write_text(PCH_CONTENT, encoding="utf8")

    stamp_path = PCH_HEADER.with_suffix(PCH_HEADER.suffix + ".sha256")
    signature = pch_signature()

    if PCH_GCH.exists() and stamp_path.exists() and stamp_path.read_text().strip() == signature:
        print(f"PCH is up to date: {PCH_GCH}")
        return 0

    command = [
        COMPILER,
        *PCH_BUILD_FLAGS,
        "-x",
        "c++-header",
        str(PCH_HEADER),
        "-o",
        str(PCH_GCH),
    ]

    print(f"Building PCH: {PCH_GCH}")
    PCH_GCH.unlink(missing_ok=True)
    subprocess.run(command, check=True)
    if not PCH_GCH.is_file():
        raise RuntimeError(f"Compiler did not create PCH: {PCH_GCH}")
    stamp_path.write_text(signature + "\n", encoding="utf8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
