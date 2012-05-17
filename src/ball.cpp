#include "ball.h"

Ball::Ball(double radius) :
    r(radius)
{
  translate = Point3D();
  rotate = Point3D();
  speed = Point3D();
  rotateSpeed = Point3D();
}
