#!/usr/bin/python
# -*- Mode: Python; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

from cmath import *
from colorsys import *

n = 8

print "Shape {"
print "geometry IndexedFaceSet {"

print "color Color {"
print "color ["

for i in range (0, n):
	h = i / float (n)
	print "%0.8f %0.8f %0.8f" % hsv_to_rgb (h, 1, 1)

print "]"
print "}"

print "coord Coordinate {"
print "point ["

for i in range (0, n):
	point = rect (1, 2*pi * i / n)
	print "%0.8f %0.8f 0" % (point .real, point .imag)

print "]"
print "}"

print "}"
print "}"

