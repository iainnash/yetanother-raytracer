//
//  Sphere.hpp
//  assign3
//
//  Created by Iain Nash on 11/30/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include "SceneObject.hpp"
#include "Utils.h"
#include "Constants.h"
#include "Ray.hpp"
#include "Color.h"

class Scene;

class Sphere : public SceneObject {
public:
  vec3 position;
  double color_diffuse[3];
  double color_specular[3];
  double shininess;
  double radius;
  
  virtual int type();
  virtual Color calc_color(Scene *s, const Ray& shadowRay, const Light& light, const ShadeHint *sh, int depth);
  virtual bool hit(const Ray& ray, float *tmin, ShadeHint *sh);
};

#endif /* Sphere_hpp */
