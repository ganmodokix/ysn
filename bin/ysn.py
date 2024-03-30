#!/usr/bin/env python3

import argparse
import os

from engine import rewrite
from run import execute

def normalize_source_path(source_path: str) -> str:
    
    base_name, ext = os.path.splitext(source_path)

    if len(ext) <= 1:
        return base_name + ".cpp"
    
    return base_name


def normalize_module_name(module_name: str) -> str:

    base_name, ext = os.path.splitext(module_name)
    return base_name + ".hpp"


if __name__ == "__main__":
    
    parser = argparse.ArgumentParser()

    parser.add_argument("source_path", type=str, help="C++ source file to be templated")
    parser.add_argument("--force", "-f", action="store_true", help="compile even though the source is not modified")
    parser.add_argument("--just", "-j", action="store_true", help="just compile, not execute")
    parser.add_argument("--install", "-i", type=str, nargs="*", help="install template if specified")
    parser.add_argument("--remove", "-r", type=str, nargs="*", help="remove template if specified")
    parser.add_argument("--dependency-graph", "-d", type=str, help="output dependency graph if specified")

    args = parser.parse_args()

    normalized_source_path = normalize_source_path(args.source_path)

    if args.install is None and args.remove is None:
        execute(normalized_source_path, just=args.just, force=args.force)
    else:
        installed_modules = args.install if args.install is not None else []
        removed_modules = args.remove if args.remove is not None else []

        installed_modules = list(map(normalize_module_name, installed_modules))
        removed_modules = list(map(normalize_module_name, removed_modules))

        rewrite(normalized_source_path, installed_modules, removed_modules)