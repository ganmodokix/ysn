import os
from typing import List
import networkx as nx
from .path import TEMPLATE_PATH

HEADER_TOKEN = "// #REQ: "

def normalize(template_id: str):
    if not template_id.endswith(".cpp"):
        return template_id + ".cpp"
    else:
        return template_id

def path_of(template_id):
    return os.path.join(TEMPLATE_PATH, normalize(template_id))

def requirements_of(template_id: str):

    with open(path_of(template_id), "r", encoding="utf8") as fp:
        line = fp.readline().strip()

    if not line.startswith(HEADER_TOKEN):
        return []
    else:
        return line[len(HEADER_TOKEN):].split()

def resolve(requirements: List[str]):

    requirements = list(map(normalize, requirements))
