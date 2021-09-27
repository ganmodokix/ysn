import os
import subprocess

from engine import normalize
from message import debug_print

import time

def exec_path_of(source_path):
    return os.path.splitext(source_path)[0] + ".exe"

def compile_if_modified(source_path):

    source_path = normalize(source_path)
    exec_path = exec_path_of(source_path)

    if os.stat(source_path).st_mtime > os.stat(exec_path).st_mtime:
        compile(source_path)

def compile(source_path):

    source_path = normalize(source_path)
    exec_path = exec_path_of(source_path)

    if not os.path.isfile(source_path):
        raise ValueError(f"{source_path} is not a file")

    debug_print(f"Compiling {source_path} ...")
    start_time = time.time()
    subprocess.run(["g++", "-std=gnu++17", "-Wall", "-Wextra", "-O2", "-DONLINE_JUDGE", source_path, "-o", exec_path])
    end_time = time.time()
    duration = start_time - end_time
    debug_print(f"Compilation done in {duration:d} ms.")

def exec(exec_path):

    subprocess.run([exec_path],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE)