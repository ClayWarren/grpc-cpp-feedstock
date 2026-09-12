import importlib
from pathlib import Path
import struct
import sys
module = importlib.import_module(sys.argv[1])
path = Path(module.__file__)
data = path.read_bytes()
assert data[:2] == b"MZ", path
pe = struct.unpack_from("<I", data, 60)[0]
assert data[pe:pe+4] == b"PE\0\0", path
assert struct.unpack_from("<H", data, pe+4)[0] == 0xAA64, path
print(f"AA64 extension verified: {path.name}")
