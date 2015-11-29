//
//  Scene.hpp
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include "Ray.hpp"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

struct Vertex
{
  vec3 position;
  double color_diffuse[3];
  double color_specular[3];
  double normal[3];
  double shininess;
};

typedef struct _Sphere
{
  vec3 position;
  double color_diffuse[3];
  double color_specular[3];
  double shininess;
  double radius;
  /*
   bool hit(Ray& ray, double& tmin, ShadeHint& sh) {
   double t;
   vec3 offset_origin = ray.o - position;
   double a = dot(ray.d, ray.d);
   double b = 2.f * ()
   
   }
   */
  
  bool intersect(Ray& ray, double *tmin) {
    vec3 vecd = position - ray.o;
    const double r2 = radius*radius;
    const float tca = dot(vecd, ray.d);
    if (tca < 0.f) return false;
    float d2 = dot(vecd, vecd) - tca * tca;
    if (d2 > r2) return false;
    float thc = sqrt(r2 - d2);
    double t0 = tca - thc;
    double t1 = tca + thc;
    *tmin = t1;
    if (t0 < t1) { *tmin = t0; }
    if (*tmin < 0.00001) {
      //sh.normal = vecd + *tmin * ray.d / radius;
      //sh.hit = ray.o + (tmin, ray.d);
    }
    return true;
  }
} Sphere;

typedef struct _Light
{
  vec3 position;
  double color[3];
} Light;

class Scene {
public:
  Triangle *triangles;
  Sphere *spheres;
  Light *lights;
  double ambient_light[3];
};


#endif /* Scene_hpp */
