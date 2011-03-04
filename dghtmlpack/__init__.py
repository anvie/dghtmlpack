
import os
from ctypes import *

curr = os.path.dirname(__file__)
dll = cdll.LoadLibrary(curr + "/htmlpacker.so")



def pack_html(data):
    
    sz = len(data)
    
    d = create_string_buffer(sz)
    
    if dll.do_html_pack(data, sz, d, sz) == 1:
        rv = d.value
        del d
        return rv
    
    print "Cannot pack this data: `%s...`, length: %d" % (data[:50], sz)
    return data



