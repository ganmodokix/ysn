from pathlib import Path
import argparse

from engine import TEMPLATE_EXTENSION_SUFFIX

if __name__ == "__main__":
    ysn_path = Path(__file__).parent.parent
    template_path = ysn_path / "template"

    parser = argparse.ArgumentParser(
        prog="lf",
        description="Find templates by a case-sensitive substring of their path."
    )
    parser.add_argument(
        "query",
        metavar="QUERY",
        help="substring to search for under template/",
    )

    args = parser.parse_args()
    query = args.query

    for candidate_path in template_path.absolute().glob("**/*" + TEMPLATE_EXTENSION_SUFFIX):
        item = str(candidate_path.relative_to(template_path))
        if query in item:
            print(item)
