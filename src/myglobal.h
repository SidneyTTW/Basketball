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
  const QString WALLS_TEXTURE_PATH[] = {":/images/0.png",
                                        ":/images/1.png",
                                        ":/images/2.png",
                                        ":/images/3.png"};
  const QString AUDIENCE_TEXTURE_PATH[] = {":/images/audience0.png",
                                           ":/images/audience1.png",
                                           ":/images/audience2.png",
                                           ":/images/audience3.png"};
  const QString COURT_TEXTURE_PATH = ":/images/lakers.png";
  const char * const SOUND_PATHS[] = {"sounds/basketballflat.wav",
                                      "sounds/basketballring.wav",
                                      "sounds/openshot.wav",
                                      "sounds/amazing.mp3",
                                      "sounds/footstep.wav"};

};

#endif // MYGLOBAL_H
