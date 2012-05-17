#include "audiencecamera.h"

#include "myglobal.h"

AudienceCamera::AudienceCamera(BasketBall *theBall,
                               Point3D audiencePos,
                               double initDistance) :
    ball(theBall),
    _fromPos(audiencePos),
    distance(initDistance)
{
}

void AudienceCamera::look() const
{
  Point3D lookAtPos = ball->translate;
  if (distance > 5)
    lookAtPos._z = MyGlobal::BASKET_HEIGHT / 2;
  Point3D direction = _fromPos - lookAtPos;
  direction = direction.normalize() * distance;
  Point3D lookFromPos = lookAtPos + direction;
  gluLookAt(lookFromPos._x,
            lookFromPos._y,
            lookFromPos._z,
            lookAtPos._x,
            lookAtPos._y,
            lookAtPos._z,
            0,
            0,
            1);
}

Point3D AudienceCamera::fromPos() const
{
  Point3D lookAtPos = ball->translate;
  if (distance > 5)
    lookAtPos._z = 0;
  Point3D direction = _fromPos - lookAtPos;
  direction = direction.normalize() * distance;
  Point3D lookFromPos = lookAtPos + direction;
  return lookFromPos;
}

Point3D AudienceCamera::toPos() const
{
  return ball->translate;
}
