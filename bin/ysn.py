#!/usr/bin/env python3

import argparse
import os

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
    
    parser = argparse.ArgumentParser(
        prog="ysn",
        description="Create, compile, run, and rewrite a templated C++ source.",
        epilog=(
            "Pass SOURCE without the .cpp extension (for example, 'ysn a' uses "
            "a.cpp). --install and --remove only rewrite the source; run ysn "
            "again without them to compile and execute. Use 'lf QUERY' to find "
            "templates and 'ly' to list the template directory tree."
        ),
    )

    parser.add_argument(
        "source_path",
        metavar="SOURCE",
        help="source path without the .cpp extension",
    )
    parser.add_argument(
        "--force", "-f", action="store_true",
        help="recompile even when the executable is newer than the source",
    )
    parser.add_argument(
        "--just", "-j", action="store_true",
        help="compile without executing",
    )
    parser.add_argument(
        "--install", "-i", metavar="MODULE", nargs="*",
        help="install templates (paths relative to template/, .hpp optional)",
    )
    parser.add_argument(
        "--remove", "-r", metavar="MODULE", nargs="*",
        help="remove installed templates (.hpp optional)",
    )

    args = parser.parse_args()

    normalized_source_path = normalize_source_path(args.source_path)

    if args.install is None and args.remove is None:
        execute(normalized_source_path, just=args.just, force=args.force)
    else:
        from engine import rewrite

        installed_modules = args.install if args.install is not None else []
        removed_modules = args.remove if args.remove is not None else []

        installed_modules = list(map(normalize_module_name, installed_modules))
        removed_modules = list(map(normalize_module_name, removed_modules))

        rewrite(normalized_source_path, installed_modules, removed_modules)
