//
//  Utils.h
//  assign3
//
//  Created by Iain Nash on 11/29/15.
//  Copyright © 2015 Iain Nash. All rights reserved.
//

#ifndef Utils_h
#define Utils_h

#define clamp2one(i) (i > 1.0 ? 1.0 : (i < 0.0 ? 0.0 : i))
#define fone2char(i) ((unsigned char)(clamp2one(i)*255))
#define vec3fromdouble(d) (vec3(d[0], d[1], d[2]))

#endif /* Utils_h */
