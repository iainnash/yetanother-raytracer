//
//  Scene.cpp
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#include "Scene.hpp"
#include <limits>
#define MAX_FLOAT std::numeric_limits<float>::max()

void parse_check(const char *expected, const char *found)
{
  if(strcasecmp(expected,found))
  {
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
  if (READ_DEBUG) printf("%s %lf %lf %lf\n",check,p[0],p[1],p[2]);
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
  if (READ_DEBUG) printf("rad: %f\n",*r);
}

void parse_shi(FILE*file,double *shi)
{
  char s[100];
  fscanf(file,"%s",s);
  parse_check("shi:",s);
  fscanf(file,"%lf",shi);
  if (READ_DEBUG) printf("shi: %f\n",*shi);
}

Color Scene::ray_to_raster(const Ray& r, unsigned int depth) {
  float raymin;
  ShadeHint hitsh;
  int ox = 0;
  int oy = 0;
  SceneObject *curobj = getClosestObject(r, &raymin, &hitsh);
  if (curobj == nullptr) {
    return BG_COLOR;
  } else {
    Color pix_col(ambient_light);
    // we've intesected an object
    #ifdef SOFT_SHADOW
    for (int i = 0; i < 4; i++) {
    for (ox = 0; ox < 2; ox++) {
    for (oy = 0; oy < 2; oy++) {
#endif
      for (auto &light : lights) {
          const Ray& shadowRay = r.createShadowRay(ox*0.2, oy*0.2, raymin, light);
          if (hasNoObjectIntersections(curobj, shadowRay)) {
            switch (curobj->type()) {
              case SceneObject::TYPE::SPHERE:
              case SceneObject::TYPE::TRIANGLE:
                pix_col += curobj->calc_color(this, shadowRay, light, &hitsh, depth + 1);
                break;
            }
          }
        }
    #ifdef SOFT_SHADOW
      } } }
      pix_col.r /= 15.0;
      pix_col.g /= 15.0;
      pix_col.b /= 15.0;
#endif
    return pix_col;
  }
}

bool Scene::hasNoObjectIntersections(const SceneObject *curobj, const Ray& r) {
  ShadeHint sh;
  float mint;
  //return true;
  // self-intersection
  // t-values (evalue t-value < 0.000001, object and light ray don't include neg t values)
  // light casts ray backwards (something between)
  for (auto &sphere : spheres) {
    if (&sphere != curobj && sphere.hit(r, &mint, &sh)) {
      if (mint > kEpsilon) {
        return false;
      }
    }
  }
  for (auto &triangle : triangles) {
    if (&triangle != curobj && triangle.hit(r, &mint, &sh)) {
      if (mint > kEpsilon*20) {
        return false;
      }
    }
  }
  return true;
}

SceneObject* Scene::getClosestObject(
    const Ray& ray, float *raymin, ShadeHint *hitsh) {
  *raymin = MAX_FLOAT;
  float tmin;
  ShadeHint sh;
  ShadeHint maxsh;
  SceneObject *curobj = NULL;
  
  for (auto &sphere : spheres) {
    if (sphere.hit(ray, &tmin, &sh)) {
      if (tmin < *raymin) {
        curobj = &sphere;
        *hitsh = sh;
        *raymin = tmin;
      }
    }
  }
  
  for (auto &triangle : triangles) {
    if (triangle.hit(ray, &tmin, &sh)) {
      if (tmin < *raymin) {
        curobj = &triangle;
        *hitsh = sh;
        *raymin = tmin;
      }
    }
  }
  
  return curobj;
}

void Scene::read_file(char *filename) {
  FILE *file = fopen(filename,"r");
  int number_of_objects;
  char type[50];
  int i;
  fscanf(file,"%i",&number_of_objects);
  
  printf("number of objects: %i\n",number_of_objects);
  
  parse_doubles(file,"amb:",ambient_light);
  
  for(i=0;i < number_of_objects;i++)
  {
    fscanf(file,"%s\n",type);
    if (READ_DEBUG) printf("%s\n",type);
    Vertex v[3];
    if(strcasecmp(type,"triangle")==0)
    {
      if (READ_DEBUG) printf("found triangle\n");
      for(int j=0;j < 3;j++)
      {
        v[j].position = parse_glm_vec3(file,"pos:");
        parse_doubles(file,"nor:",v[j].normal);
        parse_doubles(file,"dif:",v[j].color_diffuse);
        parse_doubles(file,"spe:",v[j].color_specular);
        parse_shi(file, &v[j].shininess);
      }
      triangles.push_back(Triangle(v));
    }
    else if(strcasecmp(type,"sphere")==0)
    {
      if (READ_DEBUG) printf("found sphere\n");
      Sphere s;
      s.position = parse_glm_vec3(file,"pos:");
      parse_rad(file,&s.radius);
      parse_doubles(file,"dif:",s.color_diffuse);
      parse_doubles(file,"spe:",s.color_specular);
      parse_shi(file,&s.shininess);
      spheres.push_back(s);
    }
    else if(strcasecmp(type,"light")==0)
    {
      if (READ_DEBUG) printf("found light\n");
      Light light;
      light.position = parse_glm_vec3(file,"pos:");
      parse_doubles(file,"col:",light.color);
      
      lights.push_back(light);
    }
    else
    {
      printf("unknown type in scene description:\n%s\n",type);
      exit(0);
    }
  }

}


