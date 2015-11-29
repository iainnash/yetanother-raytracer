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
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

/*
class Object : public SceneObject {

};
class Triangle : public SceneObject {

};
class Sphere : public SceneObject {

};
class Lights : public SceneObject {

};
*/

int num_triangles=0;
int num_spheres=0;
int num_lights=0;

void plot_pixel_display(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel_jpeg(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel(int x,int y,unsigned char r,unsigned char g,unsigned char b);

//MODIFY THIS FUNCTION
void draw_scene()
{
  double lookx, looky;
  Ray ray;
  ray.o = vec3(0,0,0);
  
  const int pixsize = 1;
  
  //simple output
  for(int xi=0; xi<WIDTH; xi++)
  {
    glPointSize(2.0);  
    glBegin(GL_POINTS);

    const double aspectratio = double(WIDTH) / double(HEIGHT);
    const float angle = tan(M_PI * fov / 360.f);
    //const double minLeft = -aspectratio * tan(fov * M_PI / 360.f);
    //const double minBottom = -tan(fov * M_PI / 360.f);
    //const double widthDiv = (minLeft * 2.f) / WIDTH;
    //const double heightDiv = (minBottom * 2.f) / HEIGHT;
    
    for(int yi=0; yi < HEIGHT; yi++)
    {
      double lookx = (2*((double(xi)+0.5)*1.0/WIDTH)-1) * angle * aspectratio;
      double looky = (1 - 2 * ((double(yi) + 0.5)*(1.0/HEIGHT))) * angle * aspectratio;
      //double lookx = angle * aspectratio * (yi - 0.5 * (WIDTH - 1.0));
      //double looky = angle * aspectratio * (yi - 0.5 * (HEIGHT - 1.0));
      
      //double lookx = minLeft + (double(xi) + 0.5) (;
      //double looky = minBottom + (double(yi) + 0.5);
      ray.d = normalize(vec3(lookx, looky, -1));
      
      //std::cout << "lookx: " << lookx << " looky: " << looky << std::endl;
//      vec3 raydir = normalize(vec3(lookx, looky, -1));
      //vec3 originpoint = vec3(0, 0, 0);
      
      //Ray ray(originpoint, raydir);

      for (int si = 0; si < num_triangles; si++) {
        Triangle tri = triangles[si];
        float t;
        if (tri.intersect(ray, &t)) {
          plot_pixel(xi, yi, 100, 100, 100);
        }
      }
      for (int si = 0; si < num_spheres; si++) {
        double tmin;
        if (spheres[si].intersect(ray, &tmin)) {
          plot_pixel(xi, yi, 100, 100, 100);
        }
      }
      // for (int si = 0; si < num_lights; si++) {
      //   lights[]
      // }

      //plot_pixel(x,y,x%256,y%256,(x+y)%256);
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

void parse_check(const char *expected, const char *found)
{
  if(strcasecmp(expected,found))
    {
      char error[100];
      printf("Expected '%s ' found '%s '\n",expected,found);
      printf("Parse error, abnormal abortion\n");
      exit(0);
    }

}

void parse_doubles(FILE*file, const char *check, double p[3])
{
  char str[100];
  fscanf(file,"%s",str);
  parse_check(check,str);
  fscanf(file,"%lf %lf %lf",&p[0],&p[1],&p[2]);
  printf("%s %lf %lf %lf\n",check,p[0],p[1],p[2]);
}

vec3 parse_glm_vec3(FILE*file, const char *check) {
  double doubles[3];
  parse_doubles(file, check, doubles);
  return vec3(doubles[0], doubles[1], doubles[2]);
}

void parse_rad(FILE*file,double *r)
{
  char str[100];
  fscanf(file,"%s",str);
  parse_check("rad:",str);
  fscanf(file,"%lf",r);
  printf("rad: %f\n",*r);
}

void parse_shi(FILE*file,double *shi)
{
  char s[100];
  fscanf(file,"%s",s);
  parse_check("shi:",s);
  fscanf(file,"%lf",shi);
  printf("shi: %f\n",*shi);
}

int loadScene(char *argv)
{
  FILE *file = fopen(argv,"r");
  int number_of_objects;
  char type[50];
  int i;
  Triangle t;
  Sphere s;
  Light l;
  fscanf(file,"%i",&number_of_objects);

  printf("number of objects: %i\n",number_of_objects);
  char str[200];

  parse_doubles(file,"amb:",ambient_light);

  for(i=0;i < number_of_objects;i++)
    {
      fscanf(file,"%s\n",type);
      printf("%s\n",type);
      if(strcasecmp(type,"triangle")==0)
	{

	  printf("found triangle\n");
	  int j;

	  for(j=0;j < 3;j++)
	    {
	      t.v[j].position = parse_glm_vec3(file,"pos:");
	      parse_doubles(file,"nor:",t.v[j].normal);
	      parse_doubles(file,"dif:",t.v[j].color_diffuse);
	      parse_doubles(file,"spe:",t.v[j].color_specular);
	      parse_shi(file,&t.v[j].shininess);
	    }

	  if(num_triangles == MAX_TRIANGLES)
	    {
	      printf("too many triangles, you should increase MAX_TRIANGLES!\n");
	      exit(0);
	    }
	  triangles[num_triangles++] = t;
	}
      else if(strcasecmp(type,"sphere")==0)
	{
	  printf("found sphere\n");

	  s.position = parse_glm_vec3(file,"pos:");
	  parse_rad(file,&s.radius);
	  parse_doubles(file,"dif:",s.color_diffuse);
	  parse_doubles(file,"spe:",s.color_specular);
	  parse_shi(file,&s.shininess);

	  if(num_spheres == MAX_SPHERES)
	    {
	      printf("too many spheres, you should increase MAX_SPHERES!\n");
	      exit(0);
	    }
	  spheres[num_spheres++] = s;
	}
      else if(strcasecmp(type,"light")==0)
	{
	  printf("found light\n");
	  l.position = parse_glm_vec3(file,"pos:");
	  parse_doubles(file,"col:",l.color);

	  if(num_lights == MAX_LIGHTS)
	    {
	      printf("too many lights, you should increase MAX_LIGHTS!\n");
	      exit(0);
	    }
	  lights[num_lights++] = l;
	}
      else
	{
	  printf("unknown type in scene description:\n%s\n",type);
	  exit(0);
	}
    }
  return 0;
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
  loadScene(argv[1]);

  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(WIDTH,HEIGHT);
  int window = glutCreateWindow("Ray Tracer");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  init();
  glutMainLoop();
}
