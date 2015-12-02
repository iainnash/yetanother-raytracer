#!/usr/bin/env python

from builder import *
from random import randrange

objects = [
	Light(
		position = [-1, -1, 2],
		color = [1, 0.3, 0]
	),
	Sphere(
		position = [0, 0, -35],
		diffuse_color = [0.3, 0.3, 0],
		specular_color = [0, 0.4, 0],
		radius = 1,
		shininess = 10
	)
]

for i in range(30):
	radius = randrange(5, 40)/5.0
	objects.append(Sphere(
		position = [randrange(-20, 20), randrange(-20, 20), -8 * radius],
		diffuse_color = [radius/8.0, 1.0-radius/8.0, 0.7],
		specular_color = [0.2, 0.2, 0.2],
		radius = radius,
		shininess = randrange(0, 30)
	))


write_scene('./scenes/balls_scene.scene', [0.1, 0.1, 0.1], objects)