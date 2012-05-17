#ifndef MYWORLD_H
#define MYWORLD_H

#include <QObject>
#include <QVector>
#include "point3d.h"

class Ball;
class Flat;
class Ring;

/**
 * The world used to stimulate.
 *
 * Follow things can be added into the world:
 * 1. Dynamic balls.
 * 2. Static flats in 3 directions(xy, xz, yz).
 * 3. Static ring in 1 direction(xy).
 *
 * One second is seperated into 1000 pieces.
 * Will emit some signals when some events happen.
 */
class MyWorld : public QObject
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  MyWorld();

  /**
   * Add a dynamic ball.
   */
  inline void addDynamicBall(Ball *ball)
  {dynamicBalls.append(ball);}

  /**
   * Add a static xy flat.
   */
  inline void addStaticXYFlat(Flat *flat)
  {staticXYFlats.append(flat);}

  /**
   * Add a static xz flat.
   */
  inline void addStaticXZFlat(Flat *flat)
  {staticXZFlats.append(flat);}

  /**
   * Add a static yz flat.
   */
  inline void addStaticYZFlat(Flat *flat)
  {staticYZFlats.append(flat);}

  /**
   * Add a static xy ring.
   */
  inline void addStaticXYRing(Ring *ring)
  {staticXYRings.append(ring);}

  /**
   * Advance the world in one step.
   */
  void advance();

private:
  /**
   * The dynamic balls.
   */
  QVector<Ball *> dynamicBalls;

  /**
   * The static xy flats.
   */
  QVector<Flat *> staticXYFlats;

  /**
   * The static xz flats.
   */
  QVector<Flat *> staticXZFlats;

  /**
   * The static yz flats.
   */
  QVector<Flat *> staticYZFlats;

  /**
   * The static xy flats.
   */
  QVector<Ring *> staticXYRings;

  /**
   * The time interval between 2 steps.
   * @warning Currently, the result will be changed if it's value is changed.
   */
  double timeInterval;

  /**
   * The gravity.
   */
  double gravity;

  /**
   * Try a ball will xy flat.
   *
   * @return Whether things of the ball have been changed in the function.
   */
  bool tryXYFlat(Ball *ball, Flat *flat, bool& rolled);

  /**
   * Try a ball will xz flat.
   *
   * @return Whether things of the ball have been changed in the function.
   */
  bool tryXZFlat(Ball *ball, Flat *flat);

  /**
   * Try a ball will yz flat.
   *
   * @return Whether things of the ball have been changed in the function.
   */
  bool tryYZFlat(Ball *ball, Flat *flat);

  /**
   * Try a ball will xy ring.
   *
   * @return Whether things of the ball have been changed in the function.
   */
  bool tryXYRing(Ball *ball, Ring *ring);

  /**
   * @return Whether the line from from to to passes through the ring.
   */
  bool testXYRingThrough(Point3D from, Point3D to, Ring *ring);

signals:
  /**
   * Emitted before ball bounce with a flat.
   */
  void beforeBounceBallFlat(Ball *ball, Flat *flat);

  /**
   * Emitted before ball bounce with a ring.
   */
  void beforeBounceBallRing(Ball *ball, Ring *ring);

  /**
   * Emitted after ball passes through a ring.
   */
  void ballThroughRing(Ball *ball, Ring *ring, bool hollow);
};

#endif // MYWORLD_H
