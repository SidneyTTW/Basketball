#ifndef AUDIENCECAMERA_H
#define AUDIENCECAMERA_H

#include "abstractcamera.h"

#include "basketball.h"

/**
 * Class of camera to simulate an audience.
 * Will trace the basketball.
 * With big distance, only turns left and right.
 * With small distance, will turn up and down as will, which makes the
 * basketball at the center of the view port.
 */
class AudienceCamera : public AbstractCamera
{
public:
  /**
   * Constructor.
   *
   * @param theBall The basketball to trace.
   * @param audiencePos The position of the audience.
   * @param initDistance The distance.
   */
  AudienceCamera(BasketBall *theBall, Point3D audiencePos, double initDistance);

  /**
   * Zoom in.
   */
  inline void zoomIn()
  {
    distance = qMax(ball->r * 2, distance * 0.8);
  }

  /**
   * Zoom out.
   */
  inline void zoomOut()
  {
    distance = qMin(distance * 1.125, 100.0);
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
   * The position of the audience.
   */
  Point3D _fromPos;

  /**
   * The distance.
   */
  double distance;
};

#endif // AUDIENCECAMERA_H
