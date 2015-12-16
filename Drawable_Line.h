//
//  Drawable_Line.h
//
//  Created by Hiroyuki KAMBARA on 12/02/29.
//  Copyright 2012年 Tokyo Institute of Techonology. All rights reserved.
//

#ifndef Drawable_Line_h
#define Drawable_Line_h

#include "DrawableObject.h"

class Drawable_Line : public DrawableObject {

private:
    double length;
    double width;
    
public:
    Drawable_Line(float newr, float newg, float newb, vector<float> newpos);
    Drawable_Line(const float newcolor[], vector<float> newpos);
    Drawable_Line();
    void draw();
    void setLength(double newlength);
    void setWidth(double newwidth);
    double getLength();
    double getWidth();
    
};


#endif
