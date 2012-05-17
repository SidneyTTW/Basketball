#ifndef BALL_H
#define BALL_H

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
   * The rotate.
   */
  Point3D rotate;

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
};

#endif // BALL_H
