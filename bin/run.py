import os
import subprocess

from engine import rewrite
from message import debug_print
from path import BIN_PATH

import time

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
        rewrite(source_path, [], [])

    debug_print(f"Compiling {source_path} ...")
    start_time = time.time()
    compilation = subprocess.run([
        "g++-12",
        "-std=gnu++2b",
        "-O2",
        "-Wall",
        "-Wextra",
        "-mtune=native",
        "-march=native",
        "-fconstexpr-depth=2147483647",
        "-fconstexpr-loop-limit=2147483647",
        "-fconstexpr-ops-limit=2147483647",
        "-DYSN_DEBUG",
        source_path,
        "-o", exec_path
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