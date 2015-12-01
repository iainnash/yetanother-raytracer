//
//  Scene.hpp
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include "Ray.hpp"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "SceneObject.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Vertex.h"
#include "Light.h"
#include "stdio.h"
#include <vector>

using namespace glm;



class Scene {
public:
  std::vector<Light> lights;
  std::vector<Triangle> triangles;
  std::vector<Sphere> spheres;
  double ambient_light[3];
  bool hasNoObjectIntersections(const SceneObject *o, const Ray& r);
  SceneObject* getClosestObject(const Ray& ray, float *raymin, ShadeHint *hitsh);
  void read_file(char *file);
  Color ray_to_raster(const Ray& r, unsigned int depth);
};


#endif /* Scene_hpp */
