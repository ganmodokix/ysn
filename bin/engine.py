import os
import collections
from typing import List

import networkx as nx

from path import TEMPLATE_PATH

HEADER_TOKEN = "// #REQ: "

def normalize(template_id: str):
    """
    Normalizes a template ID

    e.g., "hogehoge" -> "hogehoge.cpp"
    """
    if not template_id.endswith(".cpp"):
        return template_id + ".cpp"
    else:
        return template_id

def path_of(template_id):
    """
    Returns the path of the template with id `template_id`
    """
    return os.path.join(TEMPLATE_PATH, normalize(template_id))

dependency_cache = {}
def dependency(template_id: str):
    """
    Returns a list of the dependency of `template_id`
    """

    template_id = normalize(template_id)
    if template_id in dependency_cache:
        return dependency_cache[template_id]

    with open(path_of(template_id), "r", encoding="utf8") as fp:
        line = fp.readline().strip()

    if not line.startswith(HEADER_TOKEN):
        result = []
    else:
        result = line[len(HEADER_TOKEN):].split()

    dependency_cache[template_id] = result
    return result

def reachable_nodes_from(*dependencies: List[str]):

    dependencies = list(map(normalize, dependencies))

    queue = collections.deque(dependencies)
    nodes = set(dependencies)

    while len(queue) > 0:

        template_id = queue.popleft()

        for dependent in dependency(template_id):
            
            if dependent not in nodes:
                queue.append(dependent)
                nodes.add(dependent)

    return list(nodes)

def resolve(*dependencies: List[str]):
    """
    Resolves the dependencies
    """

    dependencies = list(map(normalize, dependencies))

    # List nodes
    nodes = reachable_nodes_from(*dependencies)

    # Resolve Dependencies
    dependency_graph = nx.DiGraph()
    dependency_graph.add_nodes_from(nodes)

    # Topological Sort
    topology = nx.topological_sort(dependency_graph)
    print(*topology)