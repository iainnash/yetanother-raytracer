//
//  Sphere.h
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Sphere_h
#define Sphere_h

#include "SceneObject.hpp"
#include "Utils.h"

class Sphere : public SceneObject {
public:
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
  
  virtual int type() {
    return TYPE::SPHERE;
  }
  
  virtual Color calc_color(const Ray& shadowRay, const Light& light, const ShadeHint *sh) {
    
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
  }
  
  virtual bool hit(const Ray& ray, float *tmin, ShadeHint *sh) {
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
    if (*tmin < kEpsilon) {
      //sh->normal = vecd + (((float)(*tmin)) * ray.d) / radius;
      //sh->hit = ray.o + (*tmin, ray.d);
    }
    sh->cam_distance = *tmin;
    return true;
  }
};

#endif /* Sphere_h */
