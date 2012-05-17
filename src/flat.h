#ifndef FLAT_H
#define FLAT_H

#include "point3d.h"

/**
 * Class of flat.
 */
class Flat
{
public:
  /**
   * Constructor.
   */
  Flat(double s1, double s2);

  /**
   * The translate.
   */
  Point3D translate;

  /**
   * The size in one direction.
   */
  double size1;

  /**
   * The size in another direction.
   */
  double size2;
};

#endif // FLAT_H
