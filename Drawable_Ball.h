//
//  Drawable_Ball.h
//
//  Created by Hiroyuki KAMBARA on 12/02/29.
//  Copyright 2012年 Tokyo Institute of Techonology. All rights reserved.
//

#ifndef Drawable_Ball_h
#define Drawable_Ball_h

#include "DrawableObject.h"


class Drawable_Ball : public DrawableObject {
    
private:
    double radius;
    static const int Nslices;
    static const int Nstacks;
    
public:
    Drawable_Ball(float newr, float newg, float newb, vector<float> newpos);
    Drawable_Ball(const float newcolor[], vector<float> newpos);
    Drawable_Ball();
    void draw();
    void setRadius(double newradius);
    double getRadius();
};



#endif
