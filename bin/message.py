import sys
import os
import inspect
from typing import Optional
from path import BIN_PATH

def debug_print(message, prefix: Optional[str] = None, err = False):
    """
    Show a debug message in the form of "[prefix] message" to stderr.
    message: message to show
    prefix: prefix to show; script path if None
    """

    call_stack = inspect.stack()
    script_path = call_stack[1].filename if len(call_stack) >= 2 else "??"
    script_path = os.path.relpath(script_path, start=BIN_PATH)
    script_line = call_stack[1].lineno

    prefix_message = f"{script_path}:{script_line}" if prefix is None else str(prefix)

    print(f"\x1b[{31 if err else 32}m\x1b[1m[{prefix_message}]\x1b[m {str(message)}", file=sys.stderr)