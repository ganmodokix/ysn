# Documentation

## ディレクトリ構造

```text
.
├── bin/                    # ysn コマンドとテンプレート展開・実行用 Python スクリプト
│   ├── ysn                 # CLI エントリポイント
│   ├── ysn.py              # 引数処理と install/remove/execute の振り分け
│   ├── engine.py           # template/ の依存解決とソースへの埋め込み処理
│   ├── run.py              # C++ ソースのコンパイル・実行処理
│   └── __base__.cpp        # 新規ソース作成時のベーステンプレート
├── template/               # 競技プログラミング用 C++ ヘッダ群
│   ├── array/              # 配列・列・累積和・Mo など
│   ├── base_template/      # 共通マクロ、型定義、入出力、デバッグ補助
│   ├── conv/               # NTT/FFT、畳み込み、各種変換
│   ├── flow/               # 最大流・最小費用流
│   ├── fps/                # 形式的冪級数
│   ├── graph/              # グラフ、木、SCC、最短路など
│   ├── marathon/           # マラソン用乱数生成器など
│   ├── math/               # 数学系テンプレート
│   ├── modint/             # modint 実装と関連補助
│   ├── monge/              # Monge 最適化
│   ├── number/             # 素数、約数、gcd、床和などの整数論
│   ├── segtree/            # セグメント木、BIT、遅延伝播、2D 構造
│   └── string/             # 文字列アルゴリズム
├── test/                   # GTest による C++ テンプレートのテスト
├── externals/
│   └── vcpkg/              # vcpkg 本体・ポート定義などの外部依存
├── .vscode/                # VS Code 用ローカル設定
├── CMakeLists.txt          # GTest 実行用 CMake 設定
├── vcpkg.json              # C++ 依存関係定義
├── pyproject.toml          # Python 依存関係定義
├── dependency.md           # テンプレート依存関係に関するメモ
├── README.md               # 基本的な使い方
└── setup.sh                # セットアップ用スクリプト
```

主に編集対象となるのは `template/`, `bin/`, `test/` とルート直下の設定・ドキュメントです。

## ツール概説

このプロジェクトの中心は、競技プログラミング用の C++ テンプレートをソースへ展開し、必要に応じてコンパイル・実行する `ysn` ツールです。
`template/` 以下のヘッダを部品として管理し、提出用ソースには必要な部品だけを依存関係込みで埋め込みます。
基本的な使い方は `bin/` 以下を `PATH` に追加しておき、`ysn`, `lf`, `ly` を短いコマンドとして直接叩く形です。

### 利用者向けコマンド

```text
ysn <source>
```

指定した C++ ソースをコンパイルして実行します。
ソース名の `.cpp` は省略でき、たとえば `ysn a` は `a.cpp` を対象にします。
ソースが存在しない場合は `bin/__base__.cpp` から新規作成し、テンプレート展開用の領域を初期化します。
実行ファイルはソース名に対応する `.exe` として作成されます。

```text
ysn <source> --install <module...>
ysn <source> --remove <module...>
ysn <source> -i <module...>
ysn <source> -r <module...>
```

ソースに埋め込むテンプレートを追加・削除します。
`--install` は `-i`、`--remove` は `-r` と省略できます。
モジュール名は `template/` からの相対パスで指定し、拡張子を省略した場合は `.hpp` として扱われます。
指定したモジュールが他のヘッダを `#include "..."` している場合、その依存も再帰的に展開されます。

```text
ysn <source> --just
ysn <source> --force
ysn <source> -j
ysn <source> -f
```

`--just` は `-j`、`--force` は `-f` と省略できます。
`-j` はコンパイルのみを行い、実行を省略します。
`-f` は更新時刻に関係なく再コンパイルします。

```text
lf <query>
ly
```

`lf` は `template/` 以下の `.hpp` から、名前に `<query>` を含むテンプレートを検索します。
`ly` は `tree` コマンドで `template/` の構造を一覧表示します。

### 内部構成

`bin/ysn` は `.env` の Python 仮想環境を有効化して `bin/ysn.py` を呼び出す薄いシェルラッパーです。
`bin/ysn.py` は CLI 引数を解釈し、テンプレート編集が必要なら `engine.py`、コンパイル・実行が必要なら `run.py` に処理を渡します。

`bin/engine.py` はテンプレート展開の中核です。
ソース内のモジュール一覧領域を読み取り、追加・削除指定を反映したうえで、`template/` 以下の `#include "..."` を依存関係として解決します。
依存グラフは DAG であることを確認し、トポロジカル順序と `PRIORITY` 指定に従って、`#pragma once` や内部 `#include` をコメント化しながら提出用ソースへ埋め込みます。

`bin/run.py` はソースの作成、コンパイル、実行を担当します。
既存の実行ファイルがなく、またはソースの更新時刻が実行ファイルより新しい場合に `g++-15` でコンパイルし、成功したら `.exe` を実行します。

`bin/find.py`, `bin/path.py`, `bin/message.py` は補助モジュールです。
`find.py` はテンプレート検索、`path.py` はプロジェクト内パスの共有、`message.py` はコンパイルや実行時のログ表示を担当します。

### セットアップと依存

`setup.sh` は `.env` を作成し、`pyproject.toml` に記述された Python 依存を導入します。
`uv` が使える環境では `uv venv` と `uv pip install` を使い、なければ標準の `venv` と `pip` にフォールバックします。
`ly` や Graphviz 出力で使う `tree` と `dot` コマンドが見つからない場合は、追加で必要なシステムコマンドとして案内します。

C++ 側のテストは `CMakeLists.txt` と `vcpkg.json` で管理されます。
`vcpkg.json` では `gtest` を依存として宣言し、`test/` 以下の `.cpp` を GTest 実行ファイルとしてビルドします。
