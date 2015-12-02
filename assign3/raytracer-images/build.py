import glob
import subprocess

paths = ['../img/*.jpg', '../animate/jpgs/*.jpg']

files = []
for path in paths:
	files += glob.glob(path)

for fi in range(len(files)):
	filename = files[fi]
	subprocess.call(['convert', '-resize', '50%', files[fi], '%03d.jpg' % fi])

print 'Done processing images'