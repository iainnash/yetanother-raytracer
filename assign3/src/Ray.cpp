//
//  Ray.cpp
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#include "Ray.hpp"
#include "Constants.h"


Ray::Ray(void) {
  
}
Ray::Ray(const vec3& o, const vec3& d) : o(o), d(d) {
  // pass
}

// static
const Ray Ray::createShadowRay(float ox, float oy, float t, Light& l) const {
  vec3 lp = l.position;
#ifdef SOFT_SHADOW
  lp.x += (RAND_FLOAT-0.5)*0.4+ox;
  lp.y += (RAND_FLOAT-0.5)*0.4+oy;
  //lp.z += (RAND_FLOAT-0.5)*0.1;
#endif
  vec3 pi = o + d * t;
  Ray shadowRay(pi, normalize(lp - pi));
  return shadowRay;
}

Ray::Ray(const Ray &obj) {
  o = obj.o;
  d = obj.d;
}
Ray::~Ray() {
  // nothing.
}