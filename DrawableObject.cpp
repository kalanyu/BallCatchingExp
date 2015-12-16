//
//  DrawableObject.cpp
//
//  Created by Hiroyuki KAMBARA on 12/02/29.
//  Copyright 2012年 Tokyo Institute of Techonology. All rights reserved.
//

#include "DrawableObject.h"


DrawableObject::DrawableObject(float newr, float newg, float newb, vector<float> newpos)
{
    pos = newpos;
    
    r = newr;
    g = newg;
    b = newb;
    a = 1.0;
}


DrawableObject::DrawableObject(const float newcolor[], vector<float> newpos)
{
    pos = newpos;
    
    for (int i=0; i<3; i++) {
        color[i] = newcolor[i];
    }
    color[3] = 1.0;
}


DrawableObject::DrawableObject()
{
    pos.resize(3,0.0);
    
    for (int i=0; i<3; i++) {
        color[i] = 0.0;
    }
    color[3] = 1.0;
}


void DrawableObject::setColor(float newr, float newg, float newb)
{
    r = newr;
    g = newg;
    b = newb;
}


void DrawableObject::setPos(vector<float> newpos)
{
    pos = newpos;
}


vector<float> DrawableObject::getPos()
{
    return pos;
}


void DrawableObject::draw()
{
}

