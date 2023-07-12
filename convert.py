import os
import ctypes
  
class ipc:
    def __enter__(self):
        self.avail = False

		# Check the library is preloaded
        if 'LD_PRELOAD' not in os.environ:
            return self
        if "/ipc.so" not in os.environ['LD_PRELOAD']:
            return self

		# Find the lib path
        elems = os.environ['LD_PRELOAD'].split(":")
        ipc_so_libpath = None
        for item in elems:
            if item.endswith("/ipc.so"):
                ipc_so_libpath = item

        if ipc_so_libpath is None:
            return self

        self.avail = True
        return self 

    def is_available(self):
        return self.avail

    def __exit__(self, exc_type, exc_val, exc_tb):
        if not self.avail:
            return
