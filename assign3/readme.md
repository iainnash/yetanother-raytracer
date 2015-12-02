Assignment #3: Ray tracing

FULL NAME: john nash

SCRIPT USAGE
------------------
* to compile: `make assign3`

* to render: `make render`

* `assign3 inscene` -> displays only
* `assign3 inscene out.jpg` -> displays and writes jpg file
* `assign3 inscene out.jpg --nodisplay` -> only writes jpg file

EXAMPLE IMAGE PATH
------------------
All numbered demo images can be found in raytracer-images.


MANDATORY FEATURES
------------------

Feature:                                 Status: finish? (yes/no)
-------------------------------------    -------------------------
1) Ray tracing triangles                  yes.

2) Ray tracing sphere                     yes.

3) Triangle Phong Shading                 yes.

4) Sphere Phong Shading                   yes.

5) Shadows rays                           yes.

6) Still images                           yes.

ex1) Soft shadows 						  yes. (see raytracer-images/002.jpg and raytracer-images/003.jpg)
ex2) Animation 							  yes. (see animate/gifs/balls.gif)
ex3) Good antialiasing 					  yes. (see all images in raytracer-images/)
ex4) Multithreaded tracer 				  yes. (see src/assign3.cpp)

7) Extra Credit (up to 20 points)

7a) Added multithreading to the raytracer / breaks into 9 images to take advantage of N cpu cores (configurable)
    decreases time from 4 seconds to 130 ms for small scenes. outputs to jpeg.
    allows for a fast high-quality render.

7b) Added (good) antialiasing using random grid sampling technique

7c) Added a set of script(s) and make files allowing for animating scenes
  	(see animate folder)
  	animation and scene building API in python to allow for code to generate scene files.
  	added makefile system to automatically build and make gifs from scenes (animate/gif) (animate/Makefile)

7e) Added soft shadows