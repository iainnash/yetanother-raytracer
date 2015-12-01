//
//  Sphere.h
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Sphere_h
#define Sphere_h

#include "Sphere.hpp"
#include "Scene.hpp"
#include "ShadeHint.hpp"
#include "Constants.h"

 int Sphere::type() {
    return TYPE::SPHERE;
  }
  
Color Sphere::calc_color(Scene *s, const Ray& shadowRay, const Light& light, const ShadeHint *sh, int depth) {
    
    vec3 inorm = (shadowRay.o - position) / float(radius);
    //if (sh->cam_distance < 0) inorm *= -1; // norm is behind camera
    
    float LdotN = clamp2one(dot(shadowRay.d, inorm));
    
    vec3 R = normalize(2.f * LdotN * inorm - shadowRay.d);
    float RdotV = clamp2one(dot(R, -normalize(shadowRay.o)));
    
    // this equation: I = lightColor * (kd * (L dot N) + ks * (R dot V) ^ sh)
    float shiny = pow(RdotV, shininess);
    
    vec3 specular = (RECURSIVE && depth < MAX_DEPTH) ? vec3(0.f, 0.f, 0.f) : shiny * vec3fromdouble(color_specular);
    
    vec3 colorI = (   (LdotN * vec3fromdouble(color_diffuse)) + specular)
    * vec3fromdouble(light.color);
  
    if (RECURSIVE && shininess > 0 && depth < MAX_DEPTH) {
      vec3 d = 2.f * (sh->normal * shadowRay.d) * (sh->normal - shadowRay.d);
      Color colorB = s->ray_to_raster(Ray(shadowRay.o, d), depth + 1);
      colorI = (1.f - float(shininess)) * colorI + float(shininess) * vec3(colorB.r, colorB.g, colorB.b);
    }
    
    return Color(colorI);
  }
  
  bool Sphere::hit(const Ray& ray, float *tmin, ShadeHint *sh) {
    vec3 vecd = position - ray.o;
    const double r2 = radius*radius;
    const float tca = dot(vecd, ray.d);
    if (fabs(tca) < 0.f) return false;
    float d2 = dot(vecd, vecd) - tca * tca;
    if (d2 > r2) return false;
    float thc = sqrt(r2 - d2);
    double t0 = tca - thc;
    double t1 = tca + thc;
    *tmin = t1;
    if (t0 < t1) { *tmin = t0; }
    sh->cam_distance = *tmin;
    sh->normal = ray.o;
    return true;
  }


#endif /* Sphere_h */
