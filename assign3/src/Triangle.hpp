//
//  Triangle.cpp
//  assign3
//
//  Created by Iain Nash on 11/30/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include "Vertex.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Constants.h"
#include "SceneObject.hpp"
#include "ShadeHint.hpp"

class Ray;
class Light;
class Scene;

using namespace glm;


class Triangle : public SceneObject {
public:
  struct Vertex v[3];
  vec3 v2mv0;
  vec3 v1mv0;
  vec3 normal;
  
  void cache_calc();
  virtual int type();
  virtual Color calc_color(Scene *s, const Ray& shadowRay, const Light& light, const ShadeHint *sh, int depth);
  Triangle(Vertex v[3]);
  virtual bool hit(const Ray& ray, float *tmin, ShadeHint *sh);
  
};



#endif /* Triangle_hpp */
