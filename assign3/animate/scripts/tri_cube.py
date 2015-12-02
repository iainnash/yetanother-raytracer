from builder import *
from random import randrange, shuffle
from itertools import permutations

objects = []

vertices = [
	[-1.0,-1.0,-1.0],
	[-1.0,-1.0, 1.0],
	[-1.0, 1.0, 1.0],
	[1.0, 1.0,-1.0],
	[-1.0,-1.0,-1.0],
	[-1.0, 1.0,-1.0],
	[1.0,-1.0, 1.0],
	[-1.0,-1.0,-1.0],
	[1.0,-1.0,-1.0],
	[1.0, 1.0,-1.0],
	[1.0,-1.0,-1.0],
	[-1.0,-1.0,-1.0],
	[-1.0,-1.0,-1.0],
	[-1.0, 1.0, 1.0],
	[-1.0, 1.0,-1.0],
	[1.0,-1.0, 1.0],
	[-1.0,-1.0, 1.0],
	[-1.0,-1.0,-1.0],
	[-1.0, 1.0, 1.0],
	[-1.0,-1.0, 1.0],
	[1.0,-1.0, 1.0],
	[1.0, 1.0, 1.0],
	[1.0,-1.0,-1.0],
	[1.0, 1.0,-1.0],
	[1.0,-1.0,-1.0],
	[1.0, 1.0, 1.0],
	[1.0,-1.0, 1.0],
	[1.0, 1.0, 1.0],
	[1.0, 1.0,-1.0],
	[-1.0, 1.0,-1.0],
	[1.0, 1.0, 1.0],
	[-1.0, 1.0,-1.0],
	[-1.0, 1.0, 1.0],
	[1.0, 1.0, 1.0],
	[-1.0, 1.0, 1.0],
	[1.0,-1.0, 1.0]]

def tr(inn):
	rn1 = randrange(-30, 30)
	return [inn[0] * rn1/2.0, inn[1] * rn1 * randrange(-2, 2)/2.3, inn[2] * rn1 - 30]
def rand_color():
	n = randrange(0,100)/100.0
	return [n, randrange(0,100)/100.0, 1-n]

for x in range(6):

	for n in range(4):
		for pv in range(0, len(vertices), 3):
			objects.append(Triangle(
				Vertex(
					position = tr(vertices[pv+0]),
					normal = [0, 1, 0.5],
					diffuse_color = rand_color(),
					specular_color = rand_color(),
					shininess = 10
				),
				Vertex(
					position = tr(vertices[pv+1]),
					normal = [0, 1, 0.5],
					diffuse_color = rand_color(),
					specular_color = rand_color(),
					shininess = 10
				),
				Vertex(
					position = tr(vertices[pv+2]),
					normal = [0, 1, 0.5],
					diffuse_color = rand_color(),
					specular_color = rand_color(),
					shininess = 10
				)
			))

	lights = [
		Light(
			position = [-1, -1, -02],
			color = [1, 0.3, 0]
		),
		Light(
			position = [0, -20, -20],
			color = [0, 0.4, 1]
		)
	]

	write_scene('./scenes/tricube%04d.scene' % x, [0.4, 0.4, 0.4], objects + lights)

