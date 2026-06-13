# ysn
Template tools for competitive programming

## Setup

Requirements:
- Python 3.11 or newer
- `g++-15` (for AtCoder-compatible compilation)
- `uv` is optional; `setup.sh` falls back to `venv` and `pip`
- `tree` is optional for `ly`
- Graphviz `dot` is optional for Graphviz output

Run the setup script from the repository root:

```
$ ./setup.sh
```

The script creates `.env` and installs the Python dependencies listed in `pyproject.toml`.
If `uv` is available, it uses `uv venv` and `uv pip install`; otherwise it uses the standard `venv` module and `pip`.

Add `bin/` to your `PATH` so the tools can be invoked with short command names:

```
$ export PATH="$PWD/bin:$PATH"
```

You may want to put the corresponding absolute path in your shell configuration.

## Usage
Runs `hoge.cpp` (create and copy the base template if not created) by
```
$ ysn hoge
```

Install or remove templates as
```
$ ysn hoge --install modint1e9p7 --remove modint998244353
```
or briefly
```
$ ysn hoge -i modint1e9p7 -r modint998244353
```

(for debigging) you can see the dependency graph by
```
$ ysn hoge -i modint1e9p7 -r modint998244353 -d hoge.svg
```
(`.svg` can be replaced by `.pdf`, `.png` etc)
