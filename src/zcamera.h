#ifndef ZCAMERA_H
#define ZCAMERA_H

#include "abstractcamera.h"

#include "basketball.h"

/**
 * Class of camera to watch the scene from sky.
 * Will trace the basketball.
 */
class ZCamera : public AbstractCamera
{
public:
  /**
   * Constructor.
   */
  ZCamera(BasketBall *theBall, double cameraT, double initDistance);

  /**
   * Zoom in.
   */
  inline void zoomIn()
  {
    distance = qMax(ball->r, distance * 0.8);
  }

  /**
   * Zoom out.
   */
  inline void zoomOut()
  {
    distance = qMin(distance * 1.125, 100.0);
  }

  /**
   * Look left.
   */
  inline void lookLeft(int speed)
  {
    t += 0.001 * speed;
  }

  /**
   * Look right.
   */
  inline void lookRight(int speed)
  {
    t -= 0.001 * speed;
  }

  virtual void look() const;
  virtual Point3D fromPos() const;
  virtual Point3D toPos() const;

private:
  /**
   * The basketball to trace.
   */
  BasketBall *ball;

  /**
   * The distance to watch.
   */
  double distance;

  /**
   * The t.
   */
  double t;
};


#endif // ZCAMERA_H
