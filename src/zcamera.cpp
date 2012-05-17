#include "zcamera.h"

#include <QtOpenGL>

ZCamera::ZCamera(BasketBall *theBall,
                 double cameraT,
                 double initDistance) :
    ball(theBall),
    t(cameraT),
    distance(initDistance)
{
}

void ZCamera::look() const
{
  double cameraZ = qMax(ball->translate._z + distance, distance * 2);
  double upX = qCos(t);
  double upY = qSin(t);
  gluLookAt(ball->translate._x, ball->translate._y, cameraZ,
            ball->translate._x, ball->translate._y, ball->translate._z,
            upX, upY, 0);
}

Point3D ZCamera::fromPos() const
{
  double cameraZ = qMax(ball->translate._z + distance, distance * 2);
  return Point3D(ball->translate._x, ball->translate._y, cameraZ);
}

Point3D ZCamera::toPos() const
{
  return ball->translate;
}
