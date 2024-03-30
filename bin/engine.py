import networkx as nx
import shutil
from pathlib import Path
import re
from collections import deque
from functools import cache
import os

MODULE_HEADER = "//  Modules below installed "
MODULE_LIST_PREFIX = "//      "
QUOTE_HEADER = "//  Quote "
QUOTE_FOOTER = "//  Unquote "
PRIORITY_PREFIX = "//  PRIORITY "
PRAGMA_ONCE_RE = re.compile("^#pragma once$")
INCLUDE_RE = re.compile('#include "(.*?)"')
INCLUDE_PATH = Path(__file__).parent.parent / "template"
BASE_SOURCE_PATH = Path(__file__).parent / "__base__.cpp"
TEMPLATE_EXTENSION_SUFFIX = ".hpp"


@cache
def fetch_dependency(module_name: str):

    dependency = []
    with open(INCLUDE_PATH / module_name, "r") as fp:
        for line in fp.readlines():
            match = re.search(INCLUDE_RE, line)
            if match is not None:
                dependency.append(match.group(1))

    return dependency


@cache
def fetch_priority(module_name: str) -> int:
    with open(INCLUDE_PATH / module_name, "r") as fp:
        for line in fp.readlines():
            if PRIORITY_PREFIX in line:
                return int(
                    line[line.index(PRIORITY_PREFIX) + len(PRIORITY_PREFIX) :].strip()
                )
    return 0


def fetch_dependency_recursively(module_names: list[str]) -> list[str]:

    visited_modules = {*module_names}
    queue = deque(visited_modules)

    while len(queue) > 0:
        module = queue.popleft()
        for dependency in fetch_dependency(module):
            if dependency in visited_modules:
                continue
            visited_modules.add(dependency)
            queue.append(dependency)

    return list(visited_modules)


def sanitize_line_for_quote(line: str, keepcomment: bool = False) -> str:
    comment_header = "// "
    if (re.match(INCLUDE_RE, line) is not None) or (
        re.match(PRAGMA_ONCE_RE, line) is not None
    ):
        line = "" if not keepcomment else comment_header + line
    return line


def generate_quote(module_names: list[str], verbose: bool = True) -> str:

    dependency_graph = nx.DiGraph()

    all_involved_modules = fetch_dependency_recursively(module_names)
    dependency_graph.add_nodes_from(all_involved_modules)

    root_node = 0
    dependency_graph.add_node(root_node)

    for module in module_names:
        dependency_graph.add_edge(root_node, module)

    for u in all_involved_modules:
        for v in fetch_dependency(u):
            dependency_graph.add_edge(u, v)

    if not nx.is_directed_acyclic_graph(dependency_graph):
        raise ValueError("Dependency must not be cyclic")

    def regularize_key(module_name):

        if module_name == root_node:
            return module_name

        priority = fetch_priority(module_name)
        priority_suffix = "\uffff" * (256 * priority)
        return priority_suffix + module_name

    topological_order = nx.lexicographical_topological_sort(
        dependency_graph, regularize_key
    )
    quote_order = list(topological_order)[::-1]

    quote_str = ""
    for header in quote_order:

        if header == root_node:
            continue

        with open(INCLUDE_PATH / header, "r") as fp:
            if verbose:
                quote_str += f"// <!-- Quote {header} -->\n"

            for line in fp.readlines():
                quote_str += sanitize_line_for_quote(line, keepcomment=True)
            if not quote_str.endswith("\n"):
                quote_str += "\n"

            if verbose:
                quote_str += f"// <!-- Unquote {header} -->\n"

    assert quote_str == "" or quote_str.endswith("\n")
    return quote_str


def parse_main_source(contents: str):

    lines = contents.splitlines(keepends=True)

    module_header_index: int | None = None
    quote_header_index: int | None = None
    quote_footer_index: int | None = None

    for index, line in enumerate(lines):

        if MODULE_HEADER in line:
            if module_header_index is not None:
                raise ValueError("Module header line is not unique")
            module_header_index = index

        if QUOTE_HEADER in line:
            if quote_header_index is not None:
                raise ValueError("Quote line is not unique")
            quote_header_index = index

        if QUOTE_FOOTER in line:
            if quote_footer_index is not None:
                raise ValueError("Unquote line is not unique")
            quote_footer_index = index

    if (
        module_header_index is None
        or quote_header_index is None
        or quote_footer_index is None
        or not module_header_index < quote_header_index < quote_footer_index
    ):

        raise ValueError(
            "directive line order invalid; expected\n"
            f"    {module_header_index=} < "
            f"{quote_header_index=} < "
            f"{quote_footer_index=}"
        )

    # list up module lists
    module_names: list[str] = []
    for module_list_line in lines[module_header_index + 1 : quote_header_index]:
        module_list_text = module_list_line.split(MODULE_LIST_PREFIX)[1].strip()
        for module_name in module_list_text.split():
            module_names.append(module_name)

    return lines[:module_header_index], module_names, lines[quote_footer_index + 1 :]


def compile_main_source(contents: str, module_list_modification=None) -> str:
    header, module_names, footer = parse_main_source(contents)

    if module_list_modification is not None:
        module_names = module_list_modification(module_names)

    result = ""

    # header
    result += "".join(header)
    if not result.endswith("\n"):
        result += "\n"

    # module list
    result += f"{MODULE_HEADER}\n"
    result += MODULE_LIST_PREFIX
    result += " ".join(module_names) + "\n"

    # quote
    result += f"{QUOTE_HEADER}\n"
    result += generate_quote(module_names)
    if not result.endswith("\n"):
        result += "\n"
    result += f"{QUOTE_FOOTER}\n"

    # footer
    result += "".join(footer)

    return result


def rewrite(
    source_path: str | Path,
    installed_modules: list[str],
    removed_modules: list[str],
    dry_run: bool = False,
):

    source_path = Path(source_path)
    if not source_path.exists():
        if not dry_run:
            shutil.copy(BASE_SOURCE_PATH, source_path)
        else:
            assert BASE_SOURCE_PATH.is_file()

    installed_modules = set(installed_modules)
    removed_modules = set(removed_modules)

    if len(installed_modules & removed_modules) > 0:
        raise ValueError(
            "No modules must be both installed and removed: "
            f"{installed_modules & removed_modules}"
        )

    if not dry_run:
        with open(source_path, "r") as fp:
            contents = fp.read()
    else:
        assert source_path.is_file()

    def modify_module_list(module_names: list[str]) -> list[str]:

        # Remove
        result = [module for module in module_names if module not in removed_modules]

        # Install
        result_set = set(result)
        for module in installed_modules:
            if module not in result_set:
                result.append(module)

        return result

    compiled_contents = compile_main_source(contents, modify_module_list)

    if not dry_run:
        with open(source_path, "w") as fp:
            fp.write(compiled_contents)
    else:
        assert os.access(source_path, os.W_OK)
