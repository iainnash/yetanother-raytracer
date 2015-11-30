/*
CSCI 480
Assignment 3 Raytracer

Name: Iain Nash
*/

#include <stdlib.h>
#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "pic.h"
#include <string.h>
#include "Ray.hpp"
#include "SceneObject.hpp"
#include "Scene.hpp"
#include <glm/vec3.hpp>
#include "ShadeHint.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Color.h"


#define MAX_TRIANGLES 2000
#define MAX_SPHERES 10
#define MAX_LIGHTS 10
using namespace glm;
using namespace std;

char *filename=NULL;

//different display modes
#define MODE_DISPLAY 1
#define MODE_JPEG 2
int mode=MODE_DISPLAY;

//you may want to make these smaller for debugging purposes
#define WIDTH 640
#define HEIGHT 480

//the field of view of the camera
#define fov 60.0

unsigned char buffer[HEIGHT][WIDTH][3];

Scene scene;


void plot_pixel_display(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel_jpeg(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel(int x,int y,unsigned char r,unsigned char g,unsigned char b);

//MODIFY THIS FUNCTION
void draw_scene()
{
  double lookx, looky;
  Ray ray;
  SceneObject *curobj;
  ray.o = vec3(0,0,0);
    
  const double aspectratio = double(WIDTH) / double(HEIGHT);
  static float angle = tan(M_PI * fov / 360.f);
  
  //simple output
  for(int xi=0; xi<WIDTH; xi++)
  {
    glPointSize(2.0);  
    glBegin(GL_POINTS);
    
    for(int yi=0; yi < HEIGHT; yi++)
    {
      lookx = (2*((double(xi)+0.5)*1.0/WIDTH)-1) * angle * aspectratio;
      looky = (1 - 2 * ((double(yi) + 0.5)*(1.0/HEIGHT))) * angle;

      ray.d = normalize(vec3(lookx, looky, -1));
      
      //std::cout << "lookx: " << lookx << " looky: " << looky << std::endl;
      float raymin;
      ShadeHint hitsh;
      curobj = scene.getClosestObject(ray, &raymin, &hitsh);
      if (curobj != nullptr) {
        Color pix_col(scene.ambient_light);
        // we've intesected an object
        for (auto light : scene.lights) {
          Ray shadowRay = ray.createShadowRay(raymin, light);
          if (scene.hasNoObjectIntersections(shadowRay)) {
            switch (curobj->type()) {
              case SceneObject::TYPE::SPHERE:
                pix_col += curobj->calc_color(shadowRay, light, &hitsh);
                break;
              case SceneObject::TYPE::TRIANGLE:
                pix_col += curobj->calc_color(shadowRay, light, &hitsh);
                break;
            }
          }
        }
        Color color = pix_col + Color(scene.ambient_light);
        plot_pixel(xi, yi, color.get_r(), color.get_g(), color.get_b());
      }
      
    }
    glEnd();
    glFlush();
  }




  printf("Done!\n"); fflush(stdout);
}


void plot_pixel_display(int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
  glColor3f(((double)r)/256.f,((double)g)/256.f,((double)b)/256.f);
  glVertex2i(x,y);
}

void plot_pixel_jpeg(int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
  buffer[HEIGHT-y-1][x][0]=r;
  buffer[HEIGHT-y-1][x][1]=g;
  buffer[HEIGHT-y-1][x][2]=b;
}

void plot_pixel(int x,int y,unsigned char r,unsigned char g, unsigned char b)
{
  plot_pixel_display(x,y,r,g,b);
  if(mode == MODE_JPEG)
      plot_pixel_jpeg(x,y,r,g,b);
}

void save_jpg()
{
  Pic *in = NULL;

  in = pic_alloc(640, 480, 3, NULL);
  printf("Saving JPEG file: %s\n", filename);

  memcpy(in->pix,buffer,3*WIDTH*HEIGHT);
  if (jpeg_write(filename, in))
    printf("File saved Successfully\n");
  else
    printf("Error in Saving\n");

  pic_free(in);      

}


void display()
{

}

void init()
{
  glMatrixMode(GL_PROJECTION);
  glOrtho(0,WIDTH,0,HEIGHT,1,-1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void idle()
{
  //hack to make it only draw once
  static int once=0;
  if(!once)
  {
      draw_scene();
      if(mode == MODE_JPEG)
	save_jpg();
    }
  once=1;
}

int main (int argc, char ** argv)
{
  if (argc<2 || argc > 3)
  {  
    printf ("usage: %s <scenefile> [jpegname]\n", argv[0]);
    exit(0);
  }
  if(argc == 3)
    {
      mode = MODE_JPEG;
      filename = argv[2];
    }
  else if(argc == 2)
    mode = MODE_DISPLAY;

  glutInit(&argc,argv);

  scene.read_file(argv[1]);

  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(WIDTH,HEIGHT);
  int window = glutCreateWindow("Ray Tracer");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  init();
  glutMainLoop();
}
