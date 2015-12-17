//
//  Drawable_Ball.cpp
//
//  Created by Hiroyuki KAMBARA on 12/02/29.
//  Copyright 2012年 Tokyo Institute of Techonology. All rights reserved.
//

#include "Drawable_Cylinder.h"

const int Drawable_Cylinder::Nslices = 30;
const int Drawable_Cylinder::Nstacks = 20;

const double default_radius = 0.05;


Drawable_Cylinder::Drawable_Cylinder(float newr, float newg, float newb, vector<float> newpos)
:DrawableObject(newr,newg,newb,newpos)
{
    radius = default_radius;
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_INSIDE);
}


Drawable_Cylinder::Drawable_Cylinder(const float newcolor[], vector<float> newpos)
:DrawableObject(newcolor,newpos)
{
    radius = default_radius;
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_INSIDE);
}


Drawable_Cylinder::Drawable_Cylinder()
:DrawableObject()
{
    radius = default_radius;
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_INSIDE);
}


void Drawable_Cylinder::draw()
{
    glPushMatrix();

    // glTranslated(pos[0], pos[1], pos[2]);
    // glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    GLUquadricObj *p = gluNewQuadric();
    gluQuadricDrawStyle(GLU_FILL);
    gluCylinder(p, 30, 30, 100, 32, 32);

    glPopMatrix();
}


void Drawable_Cylinder::setRadius(double newradius)
{
    radius = newradius;
}


double Drawable_Cylinder::getRadius()
{
    return radius;
}

Drawable_Cylinder::~Drawable_Cylinder()
{
  //gluDeleteQuadric(quadric);
}
