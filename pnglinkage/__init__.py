import sys
import os.path

sys.path.append(os.path.dirname(__file__))

import _pnglinkage

sys.path.pop()

def processPngFile(pngFile):
    return _pnglinkage.processPngFile(pngFile)
