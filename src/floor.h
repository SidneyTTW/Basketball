#ifndef FLOOR_H
#define FLOOR_H

#include "flat.h"

/**
 * Class of floor of a basketball gym.
 */
class Floor : public Flat
{
public:
  /**
   * Constructor.
   */
  Floor();

  /**
   * Destructor.
   */
  void render();

private:
  /**
   * Id of the texture.
   */
  static int texture;

  static float materialAmbient[4];
  static float materialDiffuse[4];
  static float materialSpecular[4];
};

#endif // FLOOR_H
