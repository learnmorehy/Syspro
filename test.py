import numpy as np
from convert import ipc

with ipc() as ipc:
    print("ipc Status: {}".format(ipc.is_available()))


