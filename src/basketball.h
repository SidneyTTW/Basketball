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

  static float materialAmbient[4];
  static float materialDiffuse[4];
  static float materialSpecular[4];
  static float materialShininess[1];
  static float originalShininess[1];
};

#endif // BASKETBALL_H
