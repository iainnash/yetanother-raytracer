#!/usr/bin/env python

from builder import *
from random import randrange
from math import sin, cos, tan

lights = [
	Light(
		position = [-10, -1, 20],
		color = [0.5, 0, 0.4]
	),
	Light(
		position = [-20, 20, 20],
		color = [1, 1, 0]
	),
	Sphere(
		position = [3, 3, -20],
		diffuse_color = [0, 0, 0],
		specular_color = [0.2,0.2,0.2],
		radius = 1,
		shininess = 20
	),
	Sphere(
		position = [-3, -3, -20],
		diffuse_color = [0, 0, 0],
		specular_color = [0.2, 0.2, 0.2],
		radius = 1,
		shininess = 20
	),
	Sphere(
		position = [-3, 3, -20],
		diffuse_color = [0, 0, 0],
		specular_color = [0.2, 0.2, 0.2],
		radius = 1,
		shininess = 20
	),
	Sphere(
		position = [3, -3, -20],
		diffuse_color = [0, 0, 0],
		specular_color = [0.2, 0.2, 0.2],
		radius = 1,
		shininess = 20
	)
]

objects = [

]

offset = 10.0
objz = 150
o2 = 0
for i in range(1, objz):
	ri = i / 6.0
	xp = offset * cos(ri) * i/float(objz) * 2.2
	yp = offset * sin(ri) * i/float(objz) * 2.2
	objects.append(Sphere(
		position = [xp, yp, -2 * (float(objz)/4.0)/ri - 30],
		diffuse_color = [(cos(ri+o2)+1.0)/2.0, (sin(ri+o2)+1.0)/2.0, (sin(ri+o2)+1.0)/2.0],
		specular_color = [0.2, 0.2, 0.2],
		radius = 2.8,
		shininess = 100
	))


write_scene('./scenes/corkscrew.scene', [0.3, 0.3, 0.3], objects + lights)