#!/bin/bash

if [ ! -d .env ]; then
    python3.9 -m venv .env
fi

source .env/bin/activate
sudo apt install tree graphviz -y
pip install -U pip
pip install wheel
pip install networkx graphviz