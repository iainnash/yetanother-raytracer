//
//  Ray.hpp
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Light.h"
using namespace glm;

class Ray {
public:
  vec3 o;
  vec3 d;
  Ray(void);
  Ray(const vec3& origin, const vec3& dir);
  Ray(const Ray &obj);
  ~Ray();
  const Ray createShadowRay(float md, Light& l) const;
};

class RayThrower {
  float invw;
  float invh;
  float aspectratio;
  float angle;
  Ray rayi;
public:
  RayThrower(vec3 o, int w, int h, float fov) {
    aspectratio = float(w) / float(h);
    angle = tan(M_PI * fov / 360.f);
    rayi.o = o;
    invw = 1.f/w;
    invh = 1.f/h;
  }
  const Ray& cast_fast(float x, float y) {
    float lookx = (2*((x+0.5)*invw)-1) * angle * aspectratio;
    float looky = (1 - 2 * (y+0.5)*invh) * angle;
    rayi.d = normalize(vec3(lookx, looky, -1));
    return rayi;
  }
  Ray cast(int x, int y) {
    cast_fast(x, y);
    return Ray(rayi);
  }
};

#endif /* Ray_hpp */
