import sys
import os.path

sys.path.append(os.path.dirname(__file__))

import _zliblinkage

sys.path.pop()

def compressString(s):
    return _zliblinkage.compressString(s)
