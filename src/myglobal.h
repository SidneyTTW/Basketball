#ifndef MYGLOBAL_H
#define MYGLOBAL_H

/**
 * Some global definations and frequently used fonctions.
 */

#include <QString>
#include <point3d.h>

#define equalsZero(a) (qAbs(a) < 0.00001)
#define PI            3.1415926535

/**
 * Adjust an angle into [0, PI * 2).
 */
inline void autoAdjustAngle(double& angle)
{
  double d = 2 * PI;
  while (angle < 0)
    angle += d;
  while (angle > d)
    angle -= d;
}

/**
 * @retun The angle of position (x,y)
 */
inline double calculateAngle(double x, double y)
{
  qreal result = qAtan(qAbs(y / x));
  if (x < 0 && y >= 0)
    return PI - result;
  if (x <= 0 && y < 0)
    return PI + result;
  if (x > 0 && y < 0)
    return 2 * PI - result;
  if (result != result) // NaN !!!
    return 0;
  return result;
}

/**
 * Rotate a ball according to given axis and angle.
 * It's quite an complex function.
 * Doesn't work not exactly correct but it's acceptable.
 *
 * @param f0 The f of the ball.
 * @param t0 The t of the ball.
 * @param z The z of the ball.
 * @param fa The f of the axis.
 * @param ta The t of the axis.
 * @param angle The angle to rotate.
 */
inline void rotate(double& f0, double& t0, double& z, double fa, double ta, double angle)
{
  double of0 = f0;
  if (equalsZero(angle))
    return;
  Point3D pos0 = Point3D(qSin(t0) * qCos(f0), qSin(t0) * qSin(f0), qCos(t0));
  Point3D posa = Point3D(qSin(ta) * qCos(fa), qSin(ta) * qSin(fa), qCos(ta));
  double ra = pos0._x * posa._x + pos0._y * posa._y + pos0._z * posa._z;
  if (equalsZero(cross(pos0, posa).radius()))
  {
    z += (dot(pos0, posa) > 0 ? 1 : -1) * angle;
    return;
  }
  Point3D direction0 = pos0 - posa * ra;
  direction0 = direction0.normalize();
  double rate = dot(pos0, posa);
  z += angle * rate;
  Point3D direction90 = cross(direction0, posa);
  direction90 = direction90.normalize();
  double scale = 0;
  if (angle >= 0 && angle <= PI / 2)
    scale = 1.0 / sin(3 * PI / 4 - angle) * sin(angle);
  else if (angle >= PI / 2 && angle <= PI)
  {
    scale = 1.0 / sin(5 * PI / 4 - angle) * sin(angle - PI / 2);
    Point3D tmp = direction0;
    direction0 = direction90;
    direction90 = Point3D() - tmp;
  }
  else if (angle >= PI && angle <= 3 * PI / 2)
  {
    scale = 1.0 / sin(7 * PI / 4 - angle) * sin(angle - PI);
    direction0 = Point3D() - direction0;
    direction90 = Point3D() - direction90;
  }
  else
  {
    scale = 1.0 / sin(9 * PI / 4 - angle) * sin(angle - 3 * PI / 2);
    Point3D tmp = direction0;
    direction0 = Point3D() - direction90;
    direction90 = tmp;
  }
  scale /= 1.4142;
  Point3D resultDirection = Point3D(direction0._x * (1 - scale) +
                                    direction90._x * scale,
                                    direction0._y * (1 - scale) +
                                    direction90._y * scale,
                                    direction0._z * (1 - scale) +
                                    direction90._z * scale) + posa * ra;
  resultDirection = resultDirection.normalize();
  f0 = calculateAngle(resultDirection._x, resultDirection._y);
  t0 = qAcos(resultDirection._z);
  if (t0 > PI / 2)
    z += (f0 - of0) * 2;
}

namespace MyGlobal
{
  const float BASKETBALL_RADIUS = 0.123f;
  const float BASKETBALL_COURT_LENGTH = 28.0f;
  const float BASKETBALL_COURT_WIDTH = 15.0f;
  const float REBOUND_HEIGHT = 1.05f;
  const float REBOUND_WIDTH = 1.8f;
  const float REBOUND_BOTTOM = 2.9f;
  const float BASKET_HEIGHT = 3.05f;
  const float BASKET_INNER_RADIUS = 0.143f;
  const float BASKET_OUTER_RADIUS = 0.01f;
  const float GYM_LENGTH = 30.0f;
  const float GYM_WIDTH = 16.7f;
  const float GRAVITY = 9.8f;

  const float MIN_PLAYER_HEIGHT = 1.2f;
  const float MAX_PLAYER_HEIGHT = 2.5f;

  const char BASKET_3DS_PATH[] = "basket.3ds";
  const char NET_3DS_PATH[] = "net.3ds";
  const QString BASKETBALL_TEXTURE_PATH = ":/images/basketball.png";
  const QString COURT_TEXTURE_PATH = ":/images/lakers.png";
  const char * const SOUND_PATHS[] = {"sounds/basketballflat.wav",
                                      "sounds/basketballring.wav",
                                      "sounds/openshot.wav",
                                      "sounds/amazing.mp3",
                                      "sounds/footstep.wav"};

};

#endif // MYGLOBAL_H
