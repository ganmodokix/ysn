import sys
import os
from typing import Optional
from path import BIN_PATH

def debug_print(message, prefix: Optional[str] = None, err = False):
    """
    Show a debug message in the form of "[prefix] message" to stderr.
    message: message to show
    prefix: prefix to show; script path if None
    """

    try:
        frame = sys._getframe(1)
        script_path = frame.f_code.co_filename
        script_line = frame.f_lineno
    except ValueError:
        script_path = "??"
        script_line = 0

    script_path = os.path.relpath(script_path, start=BIN_PATH)

    prefix_message = f"{script_path}:{script_line}" if prefix is None else str(prefix)

    print(f"\x1b[{31 if err else 32}m\x1b[1m[{prefix_message}]\x1b[m {str(message)}", file=sys.stderr)
