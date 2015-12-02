//
//  Light.h
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Light_h
#define Light_h

#include <glm/vec3.hpp>
#include "SceneObject.hpp"

class Light : public SceneObject {
public:
  glm::vec3 position;
  double color[3];
  virtual int type() {
    return TYPE::LIGHT;
  }
};

#endif /* Light_h */
