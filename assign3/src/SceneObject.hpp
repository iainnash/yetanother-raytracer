//
//  SceneObject.hpp
//  assign3
//
//  Created by Iain Nash on 11/28/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef SceneObject_hpp
#define SceneObject_hpp

#include "Color.h"
#include <stdexcept>

class Ray;
class Light;
class ShadeHint;
class Scene;

class SceneObject {
public:
  enum TYPE { SPHERE, TRIANGLE, LIGHT };
  virtual int type() = 0;
  virtual Color calc_color(Scene *s, const Ray& shadowRay, const Light& light, const ShadeHint *sh, int depth) {
    return Color(0.0f, 0.0f, 0.0f);
  }
  virtual bool hit(const Ray& ray, float *tmin, ShadeHint *sh) {
    throw std::runtime_error("Cannot call unimplented hit function");
  }
};

#endif /* SceneObject_hpp */
