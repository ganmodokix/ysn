#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

read_dependencies() {
    python3 - <<'PY'
import pathlib
import sys

try:
    import tomllib
except ModuleNotFoundError:
    print("Python 3.11 or newer is required to read pyproject.toml", file=sys.stderr)
    raise SystemExit(1)

pyproject = tomllib.loads(pathlib.Path("pyproject.toml").read_text())
for dependency in pyproject["project"]["dependencies"]:
    print(dependency)
PY
}

mapfile -t python_dependencies < <(read_dependencies)

if command -v uv >/dev/null 2>&1; then
    uv venv .env
    uv pip install --python .env/bin/python "${python_dependencies[@]}"
else
    if [ ! -d .env ]; then
        python3 -m venv .env
    fi

    # shellcheck disable=SC1091
    source .env/bin/activate
    python -m pip install --upgrade pip
    python -m pip install "${python_dependencies[@]}"
fi

missing_commands=()

if ! command -v tree >/dev/null 2>&1; then
    missing_commands+=("tree")
fi

if ! command -v dot >/dev/null 2>&1; then
    missing_commands+=("graphviz")
fi

if [ "${#missing_commands[@]}" -ne 0 ]; then
    printf 'Missing optional system command(s): %s\n' "${missing_commands[*]}"
    printf 'Install them with your system package manager if you use ly or Graphviz output.\n'
fi
