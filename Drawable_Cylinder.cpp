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
    gluQuadricNormals(quardric, GL_TRUE);
}


Drawable_Cylinder::Drawable_Cylinder(const float newcolor[], vector<float> newpos)
:DrawableObject(newcolor,newpos)
{
    radius = default_radius;
    quadric = gluNewQuadric();
    gluQuadricNormals(quardric, GL_TRUE);
}


Drawable_Cylinder::Drawable_Cylinder()
:DrawableObject()
{
    radius = default_radius;
    quadric = gluNewQuadric();
    gluQuadricNormals(quardric, GL_TRUE);
}


void Drawable_Cylinder::draw()
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  float radiusBase = 2;
  float radiusTop = 2;
  float height = 2;
  float slices = 20;
  float stacks = 20;

  glPushMatrix();
  glTranslatef(1.5, 0.0, 0.0);
  // glRotatef(-90.0, 1.0, 0.0, 0.0);
  glColor3f(1.0, 0.2, 0.2);	// Reddish color
  gluCylinder(quadric, radiusBase, radiusTop, height, slices, stacks);
    // Draw the top disk cap
  glPushMatrix();
  glTranslated(0.0, 0.0, height);
  gluDisk(quadric, 0.0, radiusTop, slices, stacks);
  glPopMatrix();

  // Draw the bottom disk cap
  glPushMatrix();
  glRotated(180.0, 1.0, 0.0, 0.0);
  gluDisk(quadric, 0.0, radiusBase, slices, stacks);
  glPopMatrix();

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
