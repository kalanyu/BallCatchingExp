//
//  Drawable_Ball.cpp
//
//  Created by Hiroyuki KAMBARA on 12/02/29.
//  Copyright 2012年 Tokyo Institute of Techonology. All rights reserved.
//

#include "Drawable_Ball.h"

const int Drawable_Ball::Nslices = 30;
const int Drawable_Ball::Nstacks = 20;

const double default_radius = 0.05;


Drawable_Ball::Drawable_Ball(float newr, float newg, float newb, vector<float> newpos)
:DrawableObject(newr,newg,newb,newpos)
{
    radius = default_radius;
}


Drawable_Ball::Drawable_Ball(const float newcolor[], vector<float> newpos)
:DrawableObject(newcolor,newpos)
{
    radius = default_radius;
}


Drawable_Ball::Drawable_Ball()
:DrawableObject()
{
    radius = default_radius;
}


void Drawable_Ball::draw()
{
    glPushMatrix();
    
    glTranslated(pos[0], pos[1], pos[2]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    glutSolidSphere(radius, Nslices, Nstacks);
    
    glPopMatrix();
}


void Drawable_Ball::setRadius(double newradius)
{
    radius = newradius;
}


double Drawable_Ball::getRadius()
{
    return radius;
}




