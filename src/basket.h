#ifndef BASKET_H
#define BASKET_H

#include <point3d.h>

class C3DSModel;
class Flat;
class Ring;
class MyWorld;

/**
 * Class of basket.
 */
class Basket
{
public:
  /**
   * Constructor.
   */
  Basket(Point3D translate, double t, MyWorld *world);

  /**
   * Destructor.
   */
  ~Basket();

  /**
   * Render.
   */
  void render();

  /**
   * @return The rebound.
   */
  inline Flat *getRebound()
  {
    return rebound;
  }

  /**
   * @return Other flats.
   */
  inline Flat **getOtherFlats()
  {
    return otherFlats;
  }

  /**
   * @return The ring.
   */
  inline Ring *getRing()
  {
    return ring;
  }

private:
  /**
   * The t.
   */
  double _t;

  /**
   * The position on the ground under the center of the ring.
   */
  Point3D _translate;

  /**
   * The ring.
   */
  Ring *ring;

  /**
   * The rebound.
   */
  Flat *rebound;

  /**
   * Other flats, used to describe the other parts.
   */
  Flat *otherFlats[10];

  /**
   * The model, should be init only once.
   */
  static C3DSModel *model;
};

#endif // BASKET_H
