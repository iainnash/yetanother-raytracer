//
//  ShadeHint.hpp
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef ShadeHint_hpp
#define ShadeHint_hpp

#include <glm/vec3.hpp>
using namespace glm;

class ShadeHint {
public:
  ShadeHint(vec3 normal, vec3 local_hit) : normal(normal), local_hit(local_hit) {
    // pass
  };
  ShadeHint() {
    
  };
  float tri_u;
  float tri_v;
  float cam_distance;
  vec3 normal;
  vec3 local_hit;
};

#endif /* ShadeHint_hpp */
