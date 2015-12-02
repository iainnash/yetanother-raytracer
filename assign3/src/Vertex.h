//
//  Vertex.h
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Vertex_h
#define Vertex_h

#include <glm/vec3.hpp>

struct Vertex
{
  glm::vec3 position;
  double color_diffuse[3];
  double color_specular[3];
  double normal[3];
  double shininess;
};


#endif /* Vertex_h */
