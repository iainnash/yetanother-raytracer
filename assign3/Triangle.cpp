//
//  Triangle.cpp
//  assign3
//
//  Created by Iain Nash on 11/30/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Triangle_h
#define Triangle_h

#include "Triangle.hpp"
#include "Scene.hpp"

//#define HITGOOD

using namespace glm;

/*
 virtual bool hit(const Ray& ray, float *tmin, ShadeHint *sh) {
 const vec3 pvec = cross(ray.d, v2mv0);
 const float det = dot(v1mv0, pvec);
 
 if (fabs(det) < kEpsilon) return false;
 const float invdet = 1.f/det;
 
 vec3 tvec = ray.o - v[0].position;
 float u = dot(tvec, pvec) * invdet;
 if (u < 0 || u > 1) return false;
 
 vec3 qvec = cross(tvec, v1mv0);
 float vl = dot(ray.d, qvec) * invdet;
 if (vl < 0 || u + vl > 1) return false;
 *tmin = dot(v2mv0, qvec) * invdet;
 sh->tri_u = u;
 sh->tri_v = vl;
 sh->cam_distance = *tmin;
 sh->normal = sh->tri_u*vec3fromdouble(v[0].normal) +
 sh->tri_v*vec3fromdouble(v[1].normal) +
 (1.f - sh->tri_u - sh->tri_v) * vec3fromdouble(v[2].normal);
 return true;
 }
 */

  void Triangle::cache_calc() {
    v1mv0 = v[1].position - v[0].position;
    v2mv0 = v[2].position - v[0].position;
    normal = cross(v1mv0, v2mv0);
  }
  
int Triangle::type() {
    return TYPE::TRIANGLE;
  }
  
Color Triangle::calc_color(Scene *s, const Ray& shadowRay, const Light& light, const ShadeHint *sh, int depth) {
    float tri_w = (1.f - sh->tri_u - sh->tri_v);
    
    vec3 color_diffuse = sh->tri_u*vec3fromdouble(v[0].color_diffuse) + sh->tri_v*vec3fromdouble(v[1].color_diffuse) + tri_w * vec3fromdouble(v[2].color_diffuse);
    
    vec3 color_specular = sh->tri_u*vec3fromdouble(v[0].color_specular) + sh->tri_v*vec3fromdouble(v[1].color_specular) + tri_w * vec3fromdouble(v[2].color_specular);
    
    float shininess = sh->tri_u*v[0].shininess + sh->tri_v * v[1].shininess + tri_w * v[2].shininess;
    
    float LdotN = clamp2one(dot(shadowRay.d, sh->normal));
    
    vec3 shadowDir = normalize(2.f * LdotN * sh->normal - shadowRay.d);
    float RdotV = clamp2one(dot(shadowDir, -normalize(shadowRay.o)));
    
    // this equation: I = lightColor * (kd * (L dot N) + ks * (R dot V) ^ sh)
    float shiny = pow(RdotV, shininess);
    vec3 colorI = ((LdotN * color_diffuse) + (shiny * color_specular))
    * vec3fromdouble(light.color);
    
    return Color(colorI);
  }
  
Triangle::Triangle(Vertex v[3]) {
    this->v[0] = v[0];
    this->v[1] = v[1];
    this->v[2] = v[2];
    cache_calc();
  }


bool Triangle::hit(const Ray& ray, float *tmin, ShadeHint *sh) {
  vec3 e1 = v[1].position - v[0].position;
  vec3 e2 = v[2].position - v[0].position;
  vec3 p = cross(ray.d, e2);
  float det = dot(e1, p);
  
  if (det > -kEpsilon && det < kEpsilon) return false;
  float inv_det = 1.f / det;
  
  vec3 tv = ray.o - v[0].position;
  
  sh->tri_u = dot(tv, p) * inv_det;
  if (sh->tri_u < 0.f || sh->tri_u > 1.f) return false;
  
  vec3 q = cross(tv, e1);
  sh->tri_v = dot(ray.d, q) * inv_det;
  
  if (sh->tri_v < 0.f || sh->tri_u + sh->tri_v > 1.f) return 0;
  
  float t = dot(e2, q) * inv_det;
  
  if (t > kEpsilon) {
    *tmin = t;
    sh->normal = sh->tri_u*vec3fromdouble(v[0].normal) +
    sh->tri_v*vec3fromdouble(v[1].normal) +
    (1.f - sh->tri_u - sh->tri_v) * vec3fromdouble(v[2].normal);
    
    return true;
  }
  return false;
}

/*
#ifdef HITGOOD

bool Triangle::hit(const Ray& ray, float *tmin, ShadeHint *sh) {
    const vec3 N = cross(v1mv0, v2mv0);
    float denom = dot(N, N);
    float NdotRD = dot(N, ray.d);
    if (NdotRD < kEpsilon) return false;
    float d = dot(N, v[0].position);
    *tmin = (dot(N, ray.o) + d) / NdotRD;
    if (*tmin < 0) return false;
    
    const vec3 P = ray.o + *tmin * ray.d;
    
    vec3 vp0 = P - v[0].position;
    vec3 vp1 = P - v[1].position;
    vec3 vp2 = P - v[2].position;
    vec3 edge1 = v[2].position - v[1].position;
    vec3 edge2 = v[0].position - v[2].position;
    
    if (dot(N, cross(v1mv0, vp0)) < 0) return false;
    
    if ((sh->tri_u = dot(N, cross(edge1, vp1))) < 0) return false;
    
    if ((sh->tri_v = dot(N, cross(edge2, vp2))) < 0) return false;
    
    sh->tri_u /= denom;
    sh->tri_v /= denom;
    
    return true;
  }

#else
 
bool Triangle::hit(const Ray& ray, float *tmin, ShadeHint *sh) {
  const vec3 pvec = cross(ray.d, v2mv0);
  const float det = dot(v1mv0, pvec);
   
  if (fabs(det) < kEpsilon) return false;
  const float invdet = 1.f/det;
   
  vec3 tvec = ray.o - v[0].position;
  float u = dot(tvec, pvec) * invdet;
  if (u < 0 || u > 1) return false;
   
  vec3 qvec = cross(tvec, v1mv0);
  float vl = dot(ray.d, qvec) * invdet;
  if (vl < 0 || u + vl > 1) return false;
  *tmin = dot(v2mv0, qvec) * invdet;
  sh->tri_u = u;
  sh->tri_v = vl;
  sh->cam_distance = *tmin;
  sh->normal = sh->tri_u*vec3fromdouble(v[0].normal) +
  sh->tri_v*vec3fromdouble(v[1].normal) +
   (1.f - sh->tri_u - sh->tri_v) * vec3fromdouble(v[2].normal);
  return true;
}

#endif
 */



#endif /* Triangle_h */
