#ifndef NET_H
#define NET_H

#include "3ds.h"

#include "point3d.h"

/**
 * Class of net.
 */
class Net : public C3DSModel
{
public:
  /**
   * Constructor.
   */
  Net(Point3D translate);

  /**
   * Render.
   */
  void render();

  /**
   * Set the offset of the bottom of net.
   * Will start the animation
   */
  inline void setOffset(double t, double offset)
  {
    _t = t;
    maxOffset = offset;
    anim = 0;
  }

  /**
   * Set the rotate of the net.
   */
  inline void setRotate(float rotate)
  {
    _rotate = rotate;
  }

  /**
   * Advance.
   */
  void advance();

private:
  /**
   * The position on the ground under the center of the ring.
   */
  Point3D _translate;

  /**
   * The rotate in z direction.
   */
  float _rotate;

  /**
   * Count of animation.
   * -1 means the original one.
   */
  int anim;

  /**
   * Max value of the offset.
   */
  double maxOffset;

  /**
   * The t.
   */
  double _t;

  static float materialAmbient[4];
  static float materialDiffuse[4];
  static float materialSpecular[4];
};

#endif // NET_H
