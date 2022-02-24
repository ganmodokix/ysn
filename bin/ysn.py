#!/usr/bin/env python3

import argparse

from engine import rewrite
from run import execute

if __name__ == "__main__":
    
    parser = argparse.ArgumentParser()

    parser.add_argument("source_path", type=str, help="C++ source file to be templated")
    parser.add_argument("--force", "-f", action="store_true", help="compile even though the source is not modified")
    parser.add_argument("--just", "-j", action="store_true", help="just compile, not execute")
    parser.add_argument("--install", "-i", type=str, nargs="+", help="install template if specified")
    parser.add_argument("--remove", "-r", type=str, nargs="+", help="remove template if specified")
    parser.add_argument("--dependency-graph", "-d", type=str, help="output dependency graph if specified")

    args = parser.parse_args()

    if args.install is None and args.remove is None:
        execute(args.source_path, just=args.just, force=args.force)
    else:
        rewrite(args.source_path, args.install, args.remove, args.dependency_graph)