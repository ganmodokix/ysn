# ysn
Template tools for competitive programming

## Usage
Runs `hoge.cpp` (create and copy the base template if not created) by
```
$ bin/ysn hoge
```

Install or remove templates as
```
$ bin/ysn hoge --install modint1e9p7 --remove modint998244353
```
or briefly
```
$ bin/ysn hoge -i modint1e9p7 -r modint998244353
```

(for debigging) you can see the dependency graph by
```
$ bin/ysn hoge -i modint1e9p7 -r modint998244353 -d hoge.svg
```
(`.svg` can be replaced by `.pdf`, `.png` etc)