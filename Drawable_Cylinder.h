//
//  Drawable_Ball.h
//
//  Created by Kalanyu ZINTUS-ART on 15/12/16.
//  Copyright 2015年 Tokyo Institute of Techonology. All rights reserved.
//

#ifndef Drawable_Cylinder_h
#define Drawable_Cylinder_h

#include "DrawableObject.h"

class Drawable_Cylinder : public DrawableObject {

private:
  double radius;
  double height;
  static const int Nslices;
  static const int Nstacks;
  GLUquadric *quadric;

public:
  Drawable_Cylinder(float newr, float newg, float newb, vector<float> newpos);
  Drawable_Cylinder(const float newcolor[], vector<float> newpos);
  Drawable_Cylinder();
  void draw();
  void setRadius(double newradius);
  void setHeight(double newheight);
  double getRadius();
  double getHeight();
};



#endif
