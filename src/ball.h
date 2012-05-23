#ifndef BALL_H
#define BALL_H

#include <QtOpenGL>
#include "point3d.h"

/**
 * Class of ball.
 */
class Ball
{
public:
  /**
   * Constructor.
   */
  Ball(double radius);

  /**
   * The translate.
   */
  Point3D translate;

  /**
   * The translate speed.
   */
  Point3D speed;

  /**
   * The rotate speed.
   */
  Point3D rotateSpeed;

  /**
   * The radius.
   */
  double r;

  /**
   * Rotate the ball.
   */
  void rotate(Point3D direction, double angle);

  /**
   * The matrix.
   */
  GLfloat matrix[16];
};

#endif // BALL_H
