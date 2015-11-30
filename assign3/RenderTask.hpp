//
//  RenderTask.hpp
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef RenderTask_hpp
#define RenderTask_hpp

#include "Ray.hpp"
#include "Scene.hpp"

class RenderTask {
public:
  Scene s;
  RayThrower rt;
  int ym, yi, xm, xi;
  RenderTask(Scene s, RayThrower rt, int ym, int yi, int xm, int xi) : s(s), rt(rt), ym(ym), yi(yi), xm(xm), xi(xi) {
    // pass
  }
  int xrange() {
    return xm - xi;
  }
  int yrange() {
    return ym - yi;
  }
};

#endif /* RenderTask_hpp */
