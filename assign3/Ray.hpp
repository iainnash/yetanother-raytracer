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
  Ray createShadowRay(float md, Light& l);
  /*
  Ray(const Ray& ray);
  Ray& operator=(const Ray& rhs);
  ~Ray(void);
   */
};

#endif /* Ray_hpp */
