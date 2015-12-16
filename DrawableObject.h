//
//  DrawableObject.h
//
//  Created by Hiroyuki KAMBARA on 12/02/29.
//  Copyright 2012年 Tokyo Institute of Techonology. All rights reserved.
//

#ifndef DrawableObject_h
#define DrawableObject_h

#include <vector>
using namespace std;

#include <GL/glut.h>

class DrawableObject {

protected:
    vector<float> pos;
    union {
        float color[4];
        struct {
            float r;
            float g;
            float b;
            float a;
        };
    };

public:
    DrawableObject(float newr, float newg, float newb, vector<float> newpos);
    DrawableObject(const float newcolor[], vector<float> newpos);
    DrawableObject();
    void setColor(float newr, float newg, float newb);
    void setPos(vector<float> newpos);
    vector<float> getPos();
    void draw();
};


#endif
