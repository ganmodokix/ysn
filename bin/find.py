import os
from pathlib import Path
import argparse
import glob

if __name__ == "__main__":
    ysn_path = Path(__file__).parent.parent
    template_path = ysn_path / "template"

    parser = argparse.ArgumentParser()
    parser.add_argument("query", type=str, help="search query")

    args = parser.parse_args()
    query = args.query

    for candidate_path in template_path.absolute().glob("**/*.cpp"):
        item = str(candidate_path.relative_to(template_path))
        if query in item:
            print(item)

