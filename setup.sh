#!/bin/bash

python3.9 -m venv .env

source .env/bin/activate
sudo apt install tree
pip install networkx