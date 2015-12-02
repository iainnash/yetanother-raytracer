def write(fh, header, double_list):
	fh.write(header + ': ' + (' '.join([str(float(el)) for el in double_list]) + "\n"))

def kwset(obj, args):
	for k, arg in args.items():
		if not isinstance(arg, list):
			arg = [arg]
		setattr(obj, k, arg)

class Triangle:
	def __init__(self, v1,v2,v3):
		self.vertices = [v1,v2,v3]
	def write(self, fh):
		fh.write("triangle\n")
		[v.write(fh) for v in self.vertices]

class Vertex:
	def __init__(self, **kwargs):
		kwset(self, kwargs)
	def write(self, fh):
		write(fh, 'pos', self.position)
		write(fh, 'nor', self.normal)
		write(fh, 'dif', self.diffuse_color)
		write(fh, 'spe', self.specular_color)
		write(fh, 'shi', self.shininess)

class Sphere:
	def __init__(self, **kwargs):
		kwset(self, kwargs)
	def write(self, fh):
		fh.write("sphere\n")
		write(fh, 'pos', self.position)
		write(fh, 'rad', self.radius)
		write(fh, 'dif', self.diffuse_color)
		write(fh, 'spe', self.specular_color)
		write(fh, 'shi', self.shininess)

class Light:
	def __init__(self, **kwargs):
		self.position = [0.0, 0.0, 0.0]
		self.color = [0.0, 0.0, 0.0]
		kwset(self, kwargs)
	def write(self, fh):
		fh.write("light\n")
		write(fh, 'pos', self.position)
		write(fh, 'col', self.color)


def write_scene(file, amb, objects):
	with open(file, 'w') as f:
		f.write(str(len(objects)) + "\n")
		write(f, 'amb', amb)
		for obj in objects:
			obj.write(f)