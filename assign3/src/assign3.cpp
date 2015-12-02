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
#include <pic.h>
#include <string.h>
#include "Ray.hpp"
#include "SceneObject.hpp"
#include "Scene.hpp"
#include <glm/vec3.hpp>
#include "Constants.h"
#include "ShadeHint.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Color.h"
#include <pthread.h>
#include "RenderTask.hpp"
#include <chrono>

using namespace glm;
using namespace std;

char *filename=NULL;

//different display modes
#define MODE_DISPLAY 1
#define MODE_JPEG 2
int mode=MODE_DISPLAY;

//you may want to make these smaller for debugging purposes
//#define WIDTH 1280
//#define HEIGHT 960

#define WIDTH 1280
#define HEIGHT 960

//the field of view of the camera
#define fov 60.0

unsigned char buffer[HEIGHT][WIDTH][3];

Scene scene;


void plot_pixel_display(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel_jpeg(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel(int x,int y,unsigned char r,unsigned char g,unsigned char b);

void *run_thread(void *renderproc) {
  const float num_r = 4.0;
  RenderTask *rp = (RenderTask*)renderproc;
  for (int y = 0; y < rp->yrange(); y++) {
    for (int x = 0; x < rp->xrange(); x++) {
      Color out(0.f, 0.f, 0.f);
      for (int i = 0; i < (int)num_r; i++) {
        for (int cux = 0; cux < 2; cux++) {
          for (int cuy = 0; cuy < 2; cuy++) {
            Ray ray = rp->rt.cast_fast(rp->xi + x - 0.5f + 0.25*cux + RAND_FLOAT*0.25, rp->yi + y - 0.5f + 0.25*cux + RAND_FLOAT*0.25);
            out += rp->s.ray_to_raster(ray, 1);
          }
        }
      }
      out.r /= num_r*4;
      out.g /= num_r*4;
      out.b /= num_r*4;
      buffer[rp->yi + y][rp->xi + x][0] = out.get_r();
      buffer[rp->yi + y][rp->xi + x][1] = out.get_g();
      buffer[rp->yi + y][rp->xi + x][2] = out.get_b();
    }
  }
  pthread_exit(NULL);
}

void save_jpg()
{
  Pic *in = NULL;
  
  in = pic_alloc(WIDTH, HEIGHT, 3, NULL);
  printf("Saving JPEG file: %s\n", filename);
  
  memcpy(in->pix,buffer,3*WIDTH*HEIGHT);
  if (jpeg_write(filename, in))
    printf("File saved Successfully\n");
  else
    printf("Error in Saving\n");
  
  pic_free(in);
  
}

void draw_scene_threaded(int numThreads) {
  
  int perRows = HEIGHT / numThreads;
  int perCols = WIDTH / numThreads;
  
  RayThrower thrower(vec3(0, 0, 0), WIDTH, HEIGHT, fov);
  
  vector<RenderTask*> rp_v;
  
  for (int yi = 0, yti = 0; yi <= HEIGHT - perRows; yi += perRows, yti++) {
    int ym = std::min(yi + perRows, HEIGHT);
    for (int xi = 0, xti = 0; xi <= WIDTH - perCols; xi += perCols, xti++) {
      int xm = std::min(xi + perCols, WIDTH);
      RenderTask *rp = new RenderTask(scene, thrower, ym, yi, xm, xi);
      rp_v.push_back(rp);
    }
  }
  
  pthread_attr_t attr;
  
  // Initialize and set thread joinable
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  int rc, i;
  void *status;
  
  std::chrono::milliseconds start = std::chrono::duration_cast< std::chrono::milliseconds >(
    std::chrono::system_clock::now().time_since_epoch());
  
  pthread_t *threads = new pthread_t[rp_v.size()];
  for (i = 0; i < rp_v.size(); i++) {
    rc = pthread_create(&threads[i], NULL, run_thread, (void *)rp_v[i]);
    if (rc) {
      cerr << " ERR: Unable to start thread. " << endl;
      exit(-1);
    }
  }
  
  pthread_attr_destroy(&attr);
  
  for(i = 0; i < rp_v.size(); i++){
    rc = pthread_join(threads[i], &status);
    if (rc){
      cout << "Error:unable to join," << rc << endl;
      exit(-1);
    }
  }
  
  std::chrono::milliseconds elapsed = std::chrono::duration_cast< std::chrono::milliseconds >(
    std::chrono::system_clock::now().time_since_epoch()) - start;
  
  cout << "All " << rp_v.size() << " threads done!" << endl;
  cout << "Time Elapsed: " << elapsed.count() << "ms" << endl;
  
  if (filename == NULL) filename = (char *)"default_render.jpg";
  save_jpg();

}

//MODIFY THIS FUNCTION
void draw_scene()
{
  Ray ray;
  SceneObject *curobj;
  
  RayThrower raythrower(vec3(0, 0, 0), WIDTH, HEIGHT, fov);
  
  //simple output
  for(int yi=0; yi<HEIGHT; yi++)
  {
    glPointSize(2.0);  
    glBegin(GL_POINTS);
    
    for(int xi=0; xi < WIDTH; xi++)
    {
      ray = raythrower.cast_fast(xi, yi);
      Color c = scene.ray_to_raster(ray, 1);
      plot_pixel(xi, HEIGHT-1-yi, c.get_r(), c.get_g(), c.get_b());
    }
    glEnd();
    glFlush();
  }
  printf("Done!\n");
  fflush(stdout);
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
  if (!once) {
    glPointSize(2.0);
    glBegin(GL_POINTS);

    for (int w = 0; w < WIDTH; w++) {
      for (int h = 0; h < HEIGHT; h++) {
        plot_pixel_display(w, HEIGHT-1-h, buffer[h][w][0], buffer[h][w][1], buffer[h][w][2]);
      }
    }
    glEnd();
    glFlush();
    
    if(mode == MODE_JPEG)
      save_jpg();
  
    once=1;
  }
}

int main (int argc, char ** argv)
{
  if (argc<2 || argc > 4)
  {  
    printf ("usage: %s <scenefile> [jpegname]\n", argv[0]);
    exit(0);
  }
  if(argc >= 3)
    {
      mode = MODE_JPEG;
      filename = argv[2];
    }
  else if(argc == 2)
    mode = MODE_DISPLAY;

  

  scene.read_file(argv[1]);
  
  draw_scene_threaded(3);
if (argc < 4) {  
  cout << "setting up display" << endl;
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(WIDTH,HEIGHT);
  int window = glutCreateWindow("Ray Tracer");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  init();
  glutMainLoop();
}
  
 
}
