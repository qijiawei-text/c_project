'''
This is the misc util file.
'''

import enum

@enum.unique
class StatusCode(enum.Enum):
    SUCCESS = 0
    FAIL    = -1
    WRITE_FAIL = 2
    READ_FAIL = 3
    RW_NOT_MATCH = 4
    

def three_way_cmp(x, y):
    """Return -1 if x < y, 0 if x == y and 1 if x > y"""
    return (x > y) - (x < y)


def handle_stackframe_without_leak():
    frame = inspect.currentframe()
    try:
        # do something with the frame
        print(frame)
    finally:
        del frame
