//
//  Color.h
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Color_h
#define Color_h

#include "Utils.h"
#include <glm/vec3.hpp>

class Color {
public:
  double r, g, b;
  Color(unsigned char r, unsigned char g, unsigned char b) : r(r/256), g(g/256), b(b/256) {
    // pass
  }
  Color(double r, double g, double b) : r(r), g(g), b(b) {
    // pass
  }
  Color(double d[3]) : Color(d[0], d[1], d[2]) {
    // pass
  }
  Color(glm::vec3 vec) : Color(vec.x, vec.y, vec.z) {
    // pass
  }
  unsigned char get_r() {
    return fone2char(r);
  }
  unsigned char get_g() {
    return fone2char(g);
  }
  unsigned char get_b() {
    return fone2char(b);
  }
  
  Color& operator+=(const Color &rhs) {
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    return *this;
  }
  
  const Color operator+(const Color &rhs) const {
    return Color(*this) += rhs;
  }
};

#endif /* Color_h */
