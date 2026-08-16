# ysn
Template tools for competitive programming

## Setup

Requirements:
- Python 3.11 or newer
- `g++-15` (for AtCoder-compatible compilation)
- `uv` is optional; `setup.sh` falls back to `venv` and `pip`
- `tree` is optional for `ly`

Run the setup script from the repository root:

```
$ bash setup.sh
```

The script creates `.env` and installs the Python dependencies listed in `pyproject.toml`.
If `uv` is available, it uses `uv venv` and `uv pip install`; otherwise it uses the standard `venv` module and `pip`.
It also builds a local precompiled header under `.env/pch/` to speed up repeated C++ compilations.

Add `bin/` to your `PATH` so the tools can be invoked with short command names:

```
$ export PATH="$PWD/bin:$PATH"
```

You may want to put the corresponding absolute path in your shell configuration.

## Usage
Create, compile, and run `hoge.cpp` with:

```
$ ysn hoge
```

Pass the source path without the `.cpp` extension. If `hoge.cpp` does not
exist, `ysn` creates it from the base template. The compiled executable is
written to `hoge.exe`.

Compile without running, or force recompilation, with:

```
$ ysn hoge --just
$ ysn hoge --force
```

The short forms are `-j` and `-f`, respectively.

Install or remove templates as

```
$ ysn hoge --install modint1e9p7 --remove modint998244353
```
or briefly
```
$ ysn hoge -i modint1e9p7 -r modint998244353
```

Template names are paths relative to `template/`; the `.hpp` extension is
optional. Dependencies included with `#include "..."` are expanded
recursively. Installing or removing templates only rewrites the source, so run
`ysn hoge` afterward to compile and execute it.

Search or list available templates with:

```
$ lf modint
$ ly
```
