#include "myworld.h"

#include <qmath.h>
#include "ball.h"
#include "flat.h"
#include "myglobal.h"
#include "ring.h"

#define BOUNCE_ATTENUATION 0.7
#define BOUNCE_TO_STOP     0.1
#define BOUNCE_LIMIT       0.4
#define BOUNCE_STEP        0.05
#define ROLL_ATTENUATION   0.9995
#define ROLL_TO_STOP       0.01
#define ROLL_LIMIT         0.1
#define ROLL_STEP          0.0001

inline void lowerSpeed(double& v,
                       double attenuationRate,
                       double attenuationStep,
                       double limit)
{
  if (qAbs(v) < limit)
  {
    if (abs(v) < attenuationStep)
      v = 0;
    else if (v > 0)
      v -= attenuationStep;
    else if (v < 0)
      v += attenuationStep;
  }
  else
    v *= attenuationRate;
}

inline void rotateFreeBall(Ball& ball, double timeInterval)
{
  rotate(ball.rotate._x,
         ball.rotate._y,
         ball.rotate._z,
         ball.rotateSpeed._x,
         ball.rotateSpeed._y,
         ball.rotateSpeed._z * timeInterval);
}

inline void bounceBallWithoutGravity(Ball& ball, Point3D touchPos)
{
  Point3D normal = (ball.translate - touchPos).normalize();
  Point3D axis = cross(ball.speed, normal).normalize();
  double rollSpeed = dot(ball.speed, normal);
  if (rollSpeed > 0)
    return;
  ball.speed = (ball.speed - normal * rollSpeed) * 2 - ball.speed;
  ball.rotateSpeed = Point3D(calculateAngle(axis._x, axis._y),
                             qAcos(axis._z),
                             -rollSpeed / ball.r);
  lowerSpeed(ball.speed._x, BOUNCE_ATTENUATION, 0, 0);
  lowerSpeed(ball.speed._y, BOUNCE_ATTENUATION, 0, 0);
  lowerSpeed(ball.speed._z, BOUNCE_ATTENUATION, 0, 0);
  ball.speed += normal * 0.02;
}

MyWorld::MyWorld() : timeInterval(0.001), gravity(-MyGlobal::GRAVITY)
{
}

bool MyWorld::testXYRingThrough(Point3D from, Point3D to, Ring *ring)
{
  if ((from._z >= ring->translate._z && to._z <= ring->translate._z) ||
      (from._z <= ring->translate._z && to._z >= ring->translate._z))
  {
    Point3D direction = to - from;
    double rate = (ring->translate._z  - from._z) / direction._z;
    Point3D pos = from + Point3D(direction._x * rate,
                                 direction._y * rate,
                                 direction._z * rate);
    return (pos - ring->translate).radius() < ring->R;
  }
  return false;
}

bool MyWorld::tryXYRing(Ball *ball, Ring *ring)
{
  Point3D toBePos = ball->translate + Point3D(ball->speed._x * timeInterval,
                                              ball->speed._y * timeInterval,
                                              ball->speed._z * timeInterval +
                                              gravity * timeInterval * timeInterval / 2);
  double dx = toBePos._x - ring->translate._x;
  double dy = toBePos._y - ring->translate._y;
  double dz = toBePos._z - ring->translate._z;
  double dxy = ring->R - qSqrt(dx * dx + dy * dy);
  double dis2 = dz * dz + dxy * dxy;
  bool touch = dis2 < (ball->r + ring->r) * (ball->r + ring->r);
  if (!touch)
    return false;
  emit beforeBounceBallRing(ball, ring);
  Point3D touchPos = toBePos - ring->translate;
  touchPos._z = 0;
  touchPos = touchPos.normalize();
  touchPos *= ring->R;
  touchPos += ring->translate;
  bounceBallWithoutGravity(*ball, touchPos);
  ball->translate = Point3D(ball->translate._x + ball->speed._x * timeInterval,
                            ball->translate._y + ball->speed._y * timeInterval,
                            ball->translate._z + ball->speed._z * timeInterval +
                            gravity * timeInterval * timeInterval / 2);
  ball->speed._z += gravity * timeInterval;
  return true;
}

bool MyWorld::tryXZFlat(Ball *ball, Flat *flat)
{
  Point3D& ballPos = ball->translate;
  Point3D& speed = ball->speed;
  Point3D& flatPos = flat->translate;
  double dx = ballPos._x - flatPos._x;
  double dz = ballPos._z - flatPos._z;
  double toBeY = ballPos._y + speed._y * timeInterval;
  int result = 0;
  if (ballPos._y - ball->r < flatPos._y && toBeY + ball->r > flatPos._y &&
      qAbs(dx) <= flat->size1 && qAbs(dz) <= flat->size2)
    result = -1;
  else if (ballPos._y + ball->r > flatPos._y && toBeY - ball->r < flatPos._y &&
           qAbs(dx) <= flat->size1 && qAbs(dz) <= flat->size2)
    result = 1;
  if (result == 0)
    return false;
  emit beforeBounceBallFlat(ball, flat);
  ballPos._y = flatPos._y + result * (ball->r + 0.01);
  Point3D touchPos(ball->translate._x, flat->translate._y, ball->translate._z);
  bounceBallWithoutGravity(*ball, touchPos);
  return true;
}

bool MyWorld::tryYZFlat(Ball *ball, Flat *flat)
{
  Point3D& ballPos = ball->translate;
  Point3D& speed = ball->speed;
  Point3D& flatPos = flat->translate;
  double dy = ballPos._y - flatPos._y;
  double dz = ballPos._z - flatPos._z;
  double toBeX = ballPos._x + speed._x * timeInterval;
  int result = 0;
  if (ballPos._x - ball->r < flatPos._x && toBeX + ball->r > flatPos._x &&
      qAbs(dy) <= flat->size1 && qAbs(dz) <= flat->size2)
    result = -1;
  else if (ballPos._x + ball->r > flatPos._x && toBeX - ball->r < flatPos._x &&
           qAbs(dy) <= flat->size1 && qAbs(dz) <= flat->size2)
    result = 1;
  if (result == 0)
    return false;
  emit beforeBounceBallFlat(ball, flat);
  ballPos._x = flatPos._x + result * (ball->r + 0.01);
  Point3D touchPos(flat->translate._x, ball->translate._y, ball->translate._z);
  bounceBallWithoutGravity(*ball, touchPos);
  return true;
}

bool MyWorld::tryXYFlat(Ball *ball, Flat *flat, bool& rolled)
{
  // Flat without size!!! may change to a sizable one
  Point3D& ballPos = ball->translate;
  Point3D& speed = ball->speed;
  Point3D& flatPos = flat->translate;
  double dx, dy, dz;
  dx = ballPos._x - flatPos._x;
  dy = ballPos._y - flatPos._y;
  dz = ballPos._z - flatPos._z;
  double toBeZ = ballPos._z + speed._z * timeInterval +
                 gravity * timeInterval * timeInterval / 2;
  double vxy = qSqrt(speed._x * speed._x + speed._y * speed._y);

  // touch ground
  if (equalsZero(dz - ball->r))
  {
    // no bounce
    if (equalsZero(speed._z))
    {
      // roll
      if (!equalsZero(vxy))
      {
        lowerSpeed(speed._x, ROLL_ATTENUATION, ROLL_STEP, 0);
        lowerSpeed(speed._y, ROLL_ATTENUATION, ROLL_STEP, 0);
        ball->rotateSpeed = Point3D(0, 0, 0);
        if (speed._x * speed._x  + speed._y * speed._y <
            ROLL_TO_STOP * ROLL_TO_STOP)
        {
          speed._x = 0;
          speed._y = 0;
        }
        else
        {
          ball->rotateSpeed = Point3D(calculateAngle(speed._x, speed._y) - PI / 2,
                                      PI / 2, vxy / ball->r);
          rotateFreeBall(*ball, timeInterval);
          ballPos._x += speed._x * timeInterval;
          ballPos._y += speed._y * timeInterval;
        }
        rolled = true;
        return false;
      }
      // stop
      else
      {
        // ensure the rotate speed, nothing else to do
        ball->rotateSpeed = Point3D(0, 0, 0);
        return true;
      }
    }
    // bounce
    else if (speed._z < 0)
    {
      emit beforeBounceBallFlat(ball, flat);
      {
        speed._x *= ROLL_ATTENUATION;
        speed._y *= ROLL_ATTENUATION;
        vxy = qSqrt(speed._x * speed._x + speed._y * speed._y);
        ball->rotateSpeed = Point3D(calculateAngle(speed._x, speed._y) - PI / 2,
                                    PI / 2,
                                    vxy / ball->r);
        ballPos._x += speed._x * timeInterval;
        ballPos._y += speed._y * timeInterval;
        rotateFreeBall(*ball, timeInterval);
      }
      double energy = speed._z * speed._z / 2 + (dz - ball->r) * gravity;
      ballPos._z = flatPos._z + ball->r;
      speed._z = energy >= 0 ? qSqrt(energy * 2) : 0;
      lowerSpeed(speed._z, BOUNCE_ATTENUATION, BOUNCE_STEP, BOUNCE_LIMIT);
      if (qAbs(speed._z) < BOUNCE_TO_STOP)
        speed._z = 0;
      else
        ballPos._z = qMax(qMax(ballPos._z,
                               ballPos._z +
                               speed._z * timeInterval +
                               gravity  * timeInterval * timeInterval / 2),
                          flatPos._z + ball->r);
      return true;
    }
    // bounced
    else
    {
      // free ball, won't calculate the final state here
      return false;
    }
  }
  // over ground
  else if (ballPos._z > flatPos._z)
  {
    // bounce
    if (toBeZ - ball->r < flatPos._z)
    {
      emit beforeBounceBallFlat(ball, flat);
      {
        speed._x *= ROLL_ATTENUATION;
        speed._y *= ROLL_ATTENUATION;
        vxy = qSqrt(speed._x * speed._x + speed._y * speed._y);
        ball->rotateSpeed = Point3D(calculateAngle(speed._x, speed._y) - PI / 2,
                                    PI / 2,
                                    vxy / ball->r);
        ballPos._x += speed._x * timeInterval;
        ballPos._y += speed._y * timeInterval;
        rotateFreeBall(*ball, timeInterval);
      }
      double energy = speed._z * speed._z / 2 + (dz - ball->r) * gravity;
      ballPos._z = flatPos._z + ball->r;
      speed._z = energy >= 0 ? qSqrt(energy * 2) : 0;
      lowerSpeed(speed._z, BOUNCE_ATTENUATION, BOUNCE_STEP, BOUNCE_LIMIT);
      if (qAbs(speed._z) < BOUNCE_TO_STOP)
        speed._z = 0;
      else
        ballPos._z = qMax(qMax(ballPos._z,
                               ballPos._z +
                               speed._z * timeInterval +
                               gravity  * timeInterval * timeInterval / 2),
                          flatPos._z + ball->r);
      return true;
    }
    else
    {
      // free ball, won't calculate the final state here
      return false;
    }
  }
  // under ceil
  else
  {
    // bounce
    if (toBeZ + ball->r > flatPos._z)
    {
      emit beforeBounceBallFlat(ball, flat);
      {
        speed._x *= ROLL_ATTENUATION;
        speed._y *= ROLL_ATTENUATION;
        vxy = qSqrt(speed._x * speed._x + speed._y * speed._y);
        ball->rotateSpeed = Point3D(calculateAngle(speed._x, speed._y) + PI / 2,
                                    PI / 2,
                                    vxy / ball->r);
        ballPos._x += speed._x * timeInterval;
        ballPos._y += speed._y * timeInterval;
        rotateFreeBall(*ball, timeInterval);
      }
      double energy = qMax(0.0, speed._z * speed._z / 2 + (dz + ball->r) * gravity);
      ballPos._z = flatPos._z - ball->r;
      speed._z = energy >= 0 ? -qSqrt(energy * 2) : 0;
      lowerSpeed(speed._z, BOUNCE_ATTENUATION, BOUNCE_STEP, BOUNCE_LIMIT);
      return true;
    }
    else
    {
      // free ball, won't calculate the final state here
      return false;
    }
  }
  return false;
}

void MyWorld::advance()
{
  Ball *ball;
  foreach (ball, dynamicBalls)
  {
    Point3D translate = ball->translate;
    bool finished = false;
    bool rolled = false;
    Flat *flat;
    foreach (flat, staticXYFlats)
    {
      if (finished = tryXYFlat(ball, flat, rolled))
        break;
    }
    if (finished)
      break;
    foreach (flat, staticXZFlats)
    {
      if (finished = tryXZFlat(ball, flat))
        break;
    }
    if (finished)
      break;
    foreach (flat, staticYZFlats)
    {
      if (finished = tryYZFlat(ball, flat))
        break;
    }
    if (finished)
      break;
    Ring *ring;
    foreach (ring, staticXYRings)
    {
      if (finished = tryXYRing(ball, ring))
      {
        if (testXYRingThrough(translate, ball->translate, ring))
          emit ballThroughRing(ball, ring, false);
        break;
      }
    }
    if ((!finished) && (!rolled))
    {
      ball->translate = Point3D(ball->translate._x + ball->speed._x * timeInterval,
                                ball->translate._y + ball->speed._y * timeInterval,
                                ball->translate._z + ball->speed._z * timeInterval +
                                gravity * timeInterval * timeInterval / 2);
      rotateFreeBall(*ball, timeInterval);
      ball->speed._z += gravity * timeInterval;
      foreach (ring, staticXYRings)
      {
        if (testXYRingThrough(translate, ball->translate, ring))
        {
          emit ballThroughRing(ball, ring, true);
          break;
        }
      }
    }
  }
}
