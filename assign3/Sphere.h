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
  
  virtual int type() {
    return TYPE::SPHERE;
  }
  
  virtual Color calc_color(const Ray& shadowRay, const Light& light, const ShadeHint *sh) {
    
    vec3 inorm = (shadowRay.o - position) / float(radius);
    //if (sh->cam_distance < 0) inorm *= -1; // norm is behind camera
    
    float LdotN = clamp2one(dot(shadowRay.d, inorm));
    
    vec3 R = normalize(2.f * LdotN * inorm - shadowRay.d);
    float RdotV = clamp2one(dot(R, -normalize(shadowRay.o)));
    
    // this equation: I = lightColor * (kd * (L dot N) + ks * (R dot V) ^ sh)
    float shiny = pow(RdotV, shininess);
    vec3 colorI = (   (LdotN * vec3fromdouble(color_diffuse))
                    + (shiny * vec3fromdouble(color_specular)))
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
    sh->cam_distance = *tmin;
    return true;
  }
};

#endif /* Sphere_h */
