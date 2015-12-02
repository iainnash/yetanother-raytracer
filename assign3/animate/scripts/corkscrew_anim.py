#!/usr/bin/env python

from builder import *
from random import randrange
from math import sin, cos, tan

lights = [
	Light(
		position = [-1, -1, 5],
		color = [0, 0, 1]
	),
	Light(
		position = [-20, -20, 2],
		color = [1, 1, 0]
	)
]

objects = []

offset = 10.0

o2 = 0
s = 0
for n in range(0, 12):
	o2 += 0.1
	s += 0.1
	for i in range(1, 200):
		ri = i / 6.0
		xp = offset * cos(ri + s) * i/200.0 * 2.4
		yp = offset * sin(ri - s) * i/200.0 * 2.4
		objects.append(Sphere(
			position = [xp, yp, -0.6 * 33.0/ri - 30],
			diffuse_color = [(cos(ri+o2)+1.0)/2.0, (sin(ri+o2)+1.0)/2.0, (sin(ri+o2)+1.0)/2.0],
			specular_color = [0.2, 0.2, 0.2],
			radius = 2.8,
			shininess = 100
		))
	write_scene('./scenes/animcork%03d.scene' % n, [0.3, 0.3, 0.3], objects + lights)
	objects = []



