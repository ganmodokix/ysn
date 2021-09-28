import os
import copy
import collections
from re import template
from typing import List

import networkx as nx
from message import debug_print

from path import TEMPLATE_PATH

HEADER_TOKEN = "// #REQ:"
FOOTER_TOKEN = "// #END"

def normalize(template_id: str, abs=False):
    """
    Normalizes a template ID

    e.g., "hogehoge" -> "/root/ysn/template/hogehoge.cpp"
    """

    if not template_id.endswith(".cpp"):
        result = template_id + ".cpp"
    else:
        result = template_id

    if not abs:
        result = os.path.join(TEMPLATE_PATH, result)
    else:
        result = os.path.join(os.path.curdir, result)
        result = os.path.relpath(result)

    return result

dependency_cache = {}
def dependency(template_id: str, abs=False):
    """
    Returns a list of the dependency of `template_id`
    """
    
    template_id = normalize(template_id, abs=abs)
    if template_id in dependency_cache:
        return dependency_cache[template_id]

    result = []
    with open(template_id, "r", encoding="utf8") as fp:
        for line in fp.readlines():
            line = line.strip()
            if line.startswith(HEADER_TOKEN):
                result = line[len(HEADER_TOKEN):].strip().split()
                break

    result = list(map(normalize, result))
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

    # List nodes
    nodes = reachable_nodes_from(*dependencies)

    # Resolve Dependencies
    dependency_graph = nx.DiGraph()
    dependency_graph.add_nodes_from(nodes)
    for src in nodes:
        for dst in dependency(src):
            dependency_graph.add_edge(src, dst)

    # Topological Sort
    topology = nx.topological_sort(dependency_graph)
    topology = list(topology)[::-1]
    return topology

def link(*dependencies: List[str]):

    linked = ""

    for template_id in map(normalize, dependencies):
        linked += f"// <!-- {os.path.relpath(template_id, start=TEMPLATE_PATH)} -->\n"
        with open(template_id, "r", encoding="utf8") as fp:
            linked += fp.read()
            if not linked.endswith("\n"):
                linked += "\n"
    
    return linked

def rewrite(source_path: str, installed: List[str], removed: List[str]):

    dependencies_before = dependency(source_path, abs=True)
    dependencies = copy.deepcopy(dependencies_before)

    if removed is not None:
        for template_id in removed:
            template_id = normalize(template_id)
            if template_id in dependencies:
                dependencies.remove(template_id)
    
    if installed is not None:
        for template_id in installed:
            template_id = normalize(template_id)
            if template_id not in dependencies:
                dependencies.append(template_id)

    relpaths = list(map(lambda template_id: os.path.relpath(template_id, start=TEMPLATE_PATH), dependencies))
    debug_print(f"""Installed templates: {" ".join(relpaths)}""")

    source_path = normalize(source_path, abs=True)

    with open(source_path, "r", encoding="utf8") as fp:
        
        new_source = ""
        ignoring = False

        for line in fp.readlines():

            if line.strip().startswith("using namespace std;"):
                debug_print("\x1b[3mPoet!\x1b[m")

            if ignoring:
                if line.strip().startswith(FOOTER_TOKEN):
                    ignoring = False
                    continue
            else:
                if line.strip().startswith(HEADER_TOKEN):
                    ignoring = True
                    dep_list = " ".join(relpaths)
                    new_source += f"{HEADER_TOKEN} {dep_list}\n"
                    new_source += f"// <!-- GENERATED BEGIN -->\n"
                    resolved = resolve(*dependencies)
                    new_source += link(*resolved)
                    new_source += f"// <!-- GENERATED END -->\n"
                    new_source += FOOTER_TOKEN + "\n"
                
            if not ignoring:
                new_source += line

    with open(source_path, "w", encoding="utf8") as fp:

        fp.write(new_source)

    debug_print("Successfully linked")