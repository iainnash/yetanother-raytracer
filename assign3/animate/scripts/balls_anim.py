#!/usr/bin/env python

from builder import *
from random import randrange, shuffle

objects = []

for i in range(30):
	radius = randrange(5, 40)/5.0
	objects.append(Sphere(
		position = [randrange(-20, 20), randrange(-20, 20), -8 * radius],
		diffuse_color = [radius/8.0, 1.0-radius/8.0, 0.7],
		specular_color = [0.2, 0.2, 0.2],
		radius = radius,
		shininess = randrange(0, 30)
	))

light = [
	Light(
		position = [-1, -1, 2],
		color = [1, 0.3, 0]
	),
	Light(
		position = [0, -20, -20],
		color = [0, 0.4, 1]
	)
]

for n in range(0, 20):
	shuffle(objects)
	for i in range(1, 4):
		cur_sph = objects[i]
		manip = 1.0
		if (cur_sph.position[2] < -20.0): manip = -1.0
		cur_sph.specular_color = [sc + (0.1*manip) for sc in cur_sph.specular_color]
		cur_sph.position[2] += (randrange(1, 4) * manip)
	for i in range(len(objects)):
		objects[i].position[2] -= 0.3
	write_scene('./scenes/balls_scene%03d.scene' % n, [0.1, 0.1, 0.1], objects + light)
