#ifndef RING_H
#define RING_H

#include "point3d.h"

/**
 * Class of ring.
 */
class Ring
{
public:
  /**
   * Constructor.
   */
  Ring(double bigR, double smallR);

  /**
   * The translate.
   */
  Point3D translate;

  /**
   * The big radius.
   */
  double R;

  /**
   * The small radius.
   */
  double r;
};

#endif // RING_H
