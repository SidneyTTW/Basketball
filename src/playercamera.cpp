#include "playercamera.h"

#include <QtOpenGL>

PlayerCamera::PlayerCamera(Point3D playerPos, double initT, double initF,
                           double xFrom, double xTo, double yFrom,
                           double yTo, double zFrom, double zTo) :
    pos(playerPos),
    t(initT),
    f(initF),
    _xFrom(xFrom),
    _xTo(xTo),
    _yFrom(yFrom),
    _yTo(yTo),
    _zFrom(zFrom),
    _zTo(zTo)
{
}

void PlayerCamera::look() const
{
  Point3D toPos = Point3D(qSin(t) * qCos(f), qSin(t) * qSin(f), qCos(t));
  Point3D upPos = Point3D(qSin(t - PI / 2) * qCos(f),
                          qSin(t - PI / 2) * qSin(f),
                          qCos(t - PI / 2));
  toPos += pos;
  gluLookAt(pos._x, pos._y, pos._z,
            toPos._x, toPos._y, toPos._z,
            upPos._x, upPos._y, upPos._z);
}

Point3D PlayerCamera::fromPos() const
{
  return pos;
}

Point3D PlayerCamera::toPos() const
{
  Point3D toPos = Point3D(qSin(t) * qCos(f), qSin(t) * qSin(f), qCos(t));
  return toPos + pos;
}
