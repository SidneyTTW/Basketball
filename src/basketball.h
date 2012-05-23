#ifndef BASKETBALL_H
#define BASKETBALL_H

#include "ball.h"

/**
 * Class of basketball.
 */
class BasketBall : public Ball
{
public:
  /**
   * Constructor.
   */
  BasketBall();

  /**
   * Destructor.
   */
  ~BasketBall();

  /**
   * Render.
   */
  void render();

private:
  /**
   * The quadric to paint.
   */
  GLUquadric* quadric;

  /**
   * Id of the texture.
   */
  static int texture;
};

#endif // BASKETBALL_H
