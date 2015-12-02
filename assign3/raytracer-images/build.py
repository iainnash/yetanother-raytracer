import glob
import subprocess
import os.path, time

paths = ['../img/*.jpg', '../animate/jpgs/*.jpg']

files = []
for path in paths:
	files += glob.glob(path)

for fi in range(len(files)):
	filename = files[fi]
	outname = '%03d.jpg' % fi
	r = True
	if (os.path.exists(outname)):
		ctime0 = os.path.getctime(outname)
		ctime1 = os.path.getctime(filename)
		if (ctime1 <= ctime0):
			r = False
	if r:
		print 'Processing %s' % filename
		subprocess.call(['convert', '-resize', '50%', filename, outname])

print 'Done processing images'