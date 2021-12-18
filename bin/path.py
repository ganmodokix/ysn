import os

THISFILE_PATH = os.path.abspath(__file__)
BIN_PATH = os.path.split(THISFILE_PATH)[0]
PROJ_PATH = os.path.split(BIN_PATH)[0]

TEMPLATE_PATH = os.path.join(PROJ_PATH, "template")
ACL_PATH = os.path.join(PROJ_PATH, "acl")