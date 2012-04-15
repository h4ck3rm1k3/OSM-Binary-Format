#!/usr/bin/env python2
from mapnik import *
import sys
x =int(sys.argv[1])
mapfile = 'split/style_%d.xml' % x
map_output = 'maps/layer_%d.png' % x
m = Map(4*1024,4*1024)
load_map(m, mapfile)
bbox=(Envelope( 20.271,42.06,21.804,43.125 ))
m.zoom_to_box(bbox)
print "output = " , map_output
render_to_file(m, map_output,'png256')
del m 
