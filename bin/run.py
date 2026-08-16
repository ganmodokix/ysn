import os
import subprocess
from pathlib import Path

from message import debug_print
from path import BIN_PATH, PROJ_PATH

import time

COMPILER = "g++-15"
COMPILE_FLAGS = [
    "-std=gnu++23",
    "-O2",
    "-Wall",
    "-Wextra",
    "-march=native",
    # "-flto=auto",
    "-fopenmp",
    "-pthread",
    "-ftrivial-auto-var-init=zero",
    "-fconstexpr-depth=1024",
    "-fconstexpr-loop-limit=524288",
    "-fconstexpr-ops-limit=2097152",
    "-DYSN_DEBUG",
]
PCH_BUILD_FLAGS = COMPILE_FLAGS.copy()
LINK_FLAGS = [
    "-lstdc++exp",
]
PCH_CONTENT = "#include <bits/stdc++.h>\n"
PCH_HEADER = Path(PROJ_PATH) / ".env" / "pch" / "stdcxx_all.hpp"
PCH_GCH = Path(str(PCH_HEADER) + ".gch")

def exec_path_of(source_path):
    return os.path.splitext(source_path)[0] + ".exe"

def compile_if_modified(source_path):

    exec_path = exec_path_of(source_path)

    if (not os.path.exists(exec_path)) or os.stat(source_path).st_mtime > os.stat(exec_path).st_mtime:
        compile(source_path)

def compile(source_path):

    exec_path = exec_path_of(source_path)

    if not os.path.isfile(source_path):
        
        if os.path.exists(source_path):
            raise ValueError(f"{source_path} is not a file")
        
        debug_print(f"Creating {source_path} ...")
        with open(os.path.join(BIN_PATH, "__base__.cpp"), "r", encoding="utf8") as fp:
            base_cpp = fp.read()
        with open(source_path, "w", encoding="utf8") as fp:
            fp.write(base_cpp)
        from engine import rewrite

        rewrite(source_path, [], [])

    debug_print(f"Compiling {source_path} ...")
    start_time = time.time()
    pch_flags = ["-I", str(PCH_HEADER.parent), "-include", PCH_HEADER.name] if PCH_GCH.exists() else []
    compilation = subprocess.run([
        COMPILER,
        *COMPILE_FLAGS,
        *pch_flags,
        source_path,
        "-o", exec_path,
        *LINK_FLAGS,
    ])
    end_time = time.time()
    duration = end_time - start_time

    if compilation.returncode == 0:
        debug_print(f"Compilation done in {int(duration * 1000)} ms")
    else:
        debug_print(f"Compilation failed in return code {compilation.returncode}", err=True)
        exit(compilation.returncode)

def exec(exec_path):

    debug_print(f"Executing {exec_path} ...")

    start_time = time.perf_counter()

    execution = subprocess.run([os.path.join(".", exec_path)])

    end_time = time.perf_counter()
    duration = end_time - start_time

    if execution.returncode == 0:
        debug_print(f"Successfully exited in {int(duration * 1000)} ms")
    else:
        debug_print(f"Execution failed in return code {execution.returncode}", err=True)
        exit(execution.returncode)
    
def execute(source_path, just=False, force=False):

    if force:
        compile(source_path)
    else:
        compile_if_modified(source_path)

    if not just:
        exec(exec_path_of(source_path))
