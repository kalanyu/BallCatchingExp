//
//  Drawable_Line.cpp
//
//  Created by Hiroyuki KAMBARA on 12/02/29.
//  Copyright 2012年 Tokyo Institute of Techonology. All rights reserved.
//


#include "Drawable_Line.h"

const double default_length = 1.0;
const double default_width  = 0.05;

Drawable_Line::Drawable_Line(float newr, float newg, float newb, vector<float> newpos)
:DrawableObject(newr,newg,newb,newpos)
{
    length = default_length;
    width = default_width;
}


Drawable_Line::Drawable_Line(const float newcolor[], vector<float> newpos)
:DrawableObject(newcolor,newpos)
{
    length = default_length;
    width = default_width;    
}


Drawable_Line::Drawable_Line()
:DrawableObject()
{
    length = default_length;
    width = default_width;
}


void Drawable_Line::draw()
{
    double tmp_x1 = pos[0] - length/2;
    double tmp_y1 = pos[1] - width/2;
    double tmp_x2 = pos[0] + length/2;
    double tmp_y2 = pos[1] + width/2;
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3d(tmp_x2, tmp_y1, 0);
    glVertex3d(tmp_x2, tmp_y2, 0);
    glVertex3d(tmp_x1, tmp_y1, 0);
    glVertex3d(tmp_x1, tmp_y2, 0);
    glEnd();
}


void Drawable_Line::setLength(double newlength)
{
    length = newlength;
}


double Drawable_Line::getLength()
{
    return length;
}


void Drawable_Line::setWidth(double newwidth)
{
    width = newwidth;
}


double Drawable_Line::getWidth()
{
    return width;
}

