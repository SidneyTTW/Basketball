#include "ball.h"

Ball::Ball(double radius) :
    r(radius)
{
  translate = Point3D();
  speed = Point3D();
  rotateSpeed = Point3D();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
  glPopMatrix();
}

void Ball::rotate(Point3D direction, double angle)
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glRotated(-angle, direction._x, direction._y, direction._z);

  glMultMatrixf(matrix);

  glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

  glPopMatrix();
}
