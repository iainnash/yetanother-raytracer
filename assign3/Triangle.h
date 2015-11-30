//
//  Triangle.h
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Triangle_h
#define Triangle_h

#include "Vertex.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Constants.h"
#include "ShadeHint.hpp"

using namespace glm;


class Triangle : public SceneObject {
public:
  struct Vertex v[3];
  vec3 v2mv0;
  vec3 v1mv0;
  vec3 normal;
  
  void cache_calc() {
    v2mv0 = v[2].position - v[0].position;
    v1mv0 = v[1].position - v[0].position;
    normal = cross(v1mv0, v2mv0);
  }
  
  virtual int type() {
    return TYPE::TRIANGLE;
  }
  
  virtual Color calc_color(const Ray& shadowRay, const Light& light, const ShadeHint *sh) {
    /*
    vec3 inorm = float(1.0/radius) * (shadowRay.o - position);
    if (sh->cam_distance < 0) inorm *= -1; // norm is behind camera
    
    float LdotN = clamp2one(dot(shadowRay.d, inorm));
    
    vec3 shadowDir = normalize(2.f * LdotN * inorm - shadowRay.d);
    float RdotV = clamp2one(dot(shadowDir, normalize(shadowRay.o)));
    
    // this equation: I = lightColor * (kd * (L dot N) + ks * (R dot V) ^ sh)
    vec3 colorI = (   (LdotN * vec3fromdouble(color_diffuse))
                   + (glm::pow(RdotV, float(shininess)) * vec3fromdouble(color_specular))
                   )
    * vec3fromdouble(light.color);
    
    return Color(colorI);
     */
    return Color(0.f, 0.f, 0.f);
  }
  
  Triangle(Vertex v[3]) {
    this->v[0] = v[0];
    this->v[1] = v[1];
    this->v[2] = v[2];
    cache_calc();
  }
  
  virtual bool hit(const Ray& ray, float *tmin, ShadeHint *sh) {
    vec3 pvec = cross(ray.d, v2mv0);
    float det = dot(v1mv0, pvec);
    
    if (det < kEpsilon) return false;
    
    float invdet = 1/det;
    
    vec3 tvec = ray.o - v[0].position;
    float u = dot(tvec, pvec) * invdet;
    if (u < 0 || u > 1) return false;
    
    vec3 qvec = cross(tvec, v1mv0);
    float v = dot(ray.d, qvec) * invdet;
    if (v < 0 || u + v > 1) return false;
    *tmin = dot(v2mv0, qvec) * invdet;
    sh->normal = normal;
    sh->local_hit = ray.o + *tmin * ray.d;
    sh->cam_distance = *tmin;
    return true;
  }
};



#endif /* Triangle_h */
