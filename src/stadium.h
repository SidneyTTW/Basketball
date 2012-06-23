#ifndef STADIUM_H
#define STADIUM_H

#include <point3d.h>

class C3DSModel;

class Stadium
{
public:
  Stadium(Point3D translate);

  void render();

private:
  Point3D _translate;

  static C3DSModel *model;

  static float materialAmbient[4];
  static float materialDiffuse[4];
  static float materialSpecular[4];
};

#endif // STADIUM_H
