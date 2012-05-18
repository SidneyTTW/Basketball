#ifndef PLAYERCAMERA_H
#define PLAYERCAMERA_H

#include "abstractcamera.h"

#include "myglobal.h"
#include "point3d.h"

/**
 * Class of camera to simulate a player.
 * Like pfs game without jump.
 */
class PlayerCamera : public AbstractCamera
{
public:
  /**
   * Constructor.
   */
  PlayerCamera(Point3D playerPos, double initT, double initF,
               double xFrom, double xTo, double yFrom,
               double yTo, double zFrom, double zTo);

  /**
   * Look up.
   */
  inline void lookUp(int speed)
  {
    t = qBound(0.0, t - 0.001 * speed, PI);
  }

  /**
   * Look up.
   */
  inline void lookUp(double angle)
  {
    t = qBound(0.0, t - angle, PI);
  }

  /**
   * Look down.
   */
  inline void lookDown(int speed)
  {
    t = qBound(0.0, t + 0.001 * speed, PI);
  }

  /**
   * Look down.
   */
  inline void lookDown(double angle)
  {
    t = qBound(0.0, t + angle, PI);
  }

  /**
   * Look left.
   */
  inline void lookLeft(int speed)
  {
    f += 0.002 * speed;
  }

  /**
   * Look left.
   */
  inline void lookLeft(double angle)
  {
    f += angle;
  }

  /**
   * Look right.
   */
  inline void lookRight(int speed)
  {
    f -= 0.002 * speed;
  }

  /**
   * Look right.
   */
  inline void lookRight(double angle)
  {
    f -= angle;
  }

  /**
   * Move up.
   */
  inline void moveUp(int speed)
  {
    pos._z = qBound(_zFrom, pos._z + 0.002 * speed, _zTo);
  }

  /**
   * Move down.
   */
  inline void moveDown(int speed)
  {
    pos._z = qBound(_zFrom, pos._z - 0.002 * speed, _zTo);
  }

  /**
   * Move forward.
   */
  inline void moveForward(int speed)
  {
    Point3D forward = Point3D(qCos(f), qSin(f), 0).normalize();
    pos += forward * 0.005 * speed;
    pos._x = qBound(_xFrom, pos._x, _xTo);
    pos._y = qBound(_yFrom, pos._y, _yTo);
  }

  /**
   * Move backward.
   */
  inline void moveBackward(int speed)
  {
    Point3D forward = Point3D(qCos(f), qSin(f), 0).normalize();
    pos -= forward * 0.005 * speed;
    pos._x = qBound(_xFrom, pos._x, _xTo);
    pos._y = qBound(_yFrom, pos._y, _yTo);
  }

  /**
   * Move left.
   */
  inline void moveLeft(int speed)
  {
    Point3D left = cross(Point3D(0, 0, 1), Point3D(qCos(f), qSin(f), 0)).normalize();
    pos += left * 0.005 * speed;
    pos._x = qBound(_xFrom, pos._x, _xTo);
    pos._y = qBound(_yFrom, pos._y, _yTo);
  }

  /**
   * Move right.
   */
  inline void moveRight(int speed)
  {
    Point3D left = cross(Point3D(0, 0, -1), Point3D(qCos(f), qSin(f), 0)).normalize();
    pos += left * 0.005 * speed;
    pos._x = qBound(_xFrom, pos._x, _xTo);
    pos._y = qBound(_yFrom, pos._y, _yTo);
  }

  /**
   * Get the t.
   */
  inline double getT() const
  {
    return t;
  }

  /**
   * Get the f.
   */
  inline double getF() const
  {
    return f;
  }

  /**
   * Get the position.
   */
  inline Point3D getPos() const
  {
    return pos;
  }

  virtual void look() const;
  virtual Point3D fromPos() const;
  virtual Point3D toPos() const;

private:
  /**
   * The position.
   */
  Point3D pos;

  /**
   * The t.
   */
  double t;

  /**
   * The f.
   */
  double f;

  /**
   * The bound.
   */
  double _xFrom;

  /**
   * The bound.
   */
  double _xTo;

  /**
   * The bound.
   */
  double _yFrom;

  /**
   * The bound.
   */
  double _yTo;

  /**
   * The bound.
   */
  double _zFrom;

  /**
   * The bound.
   */
  double _zTo;
};

#endif // PLAYERCAMERA_H
