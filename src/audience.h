#ifndef AUDIENCE_H
#define AUDIENCE_H

#include "point3d.h"

/**
 * Class of an audience.
 */
class Audience
{
public:
  Audience(Point3D translate, double t);

  void render();

  /**
   * The t.
   */
  double _t;

  /**
   * The translate.
   */
  Point3D _translate;

private:
  int id;

  /**
   * Ids of the textures.
   */
  static int *textures;

  static float materialAmbient[4];
  static float materialDiffuse[4];
  static float materialSpecular[4];
};

#endif // AUDIENCE_H
