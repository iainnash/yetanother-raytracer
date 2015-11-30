//
//  Ray.cpp
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#include "Ray.hpp"


Ray::Ray(void) {
  
}
Ray::Ray(const vec3& o, const vec3& d) : o(o), d(d) {
  // pass
}

// static
Ray Ray::createShadowRay(float md, Light& l) {
  vec3 pi = o + d * md;
  Ray shadowRay(pi, normalize(l.position - pi));
  return shadowRay;
}

Ray::Ray(const Ray &obj) {
  o = obj.o;
  d = obj.d;
}
Ray::~Ray() {
  // nothing.
}