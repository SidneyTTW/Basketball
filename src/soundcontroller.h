#ifndef SOUNDCONTROLLER_H
#define SOUNDCONTROLLER_H

#include <QList>
#include <fmod.h>
#include "point3d.h"

/**
 * Class to control sounds.
 */
class SoundController
{
public:
  enum BuiltInSoundType {BounceFlat = 0,
                         BounceBasket,
                         OpenShot};

  struct SoundUnit
  {
    FSOUND_SAMPLE *sound;
    Point3D position;
    double volumn;
    int channel;
  };

  static void init();

  static void playSound
      (BuiltInSoundType type, Point3D position, double volumn);

  static void setListennerPosition(Point3D position, Point3D direction);

  static FSOUND_SAMPLE *footstep;

  static int footstepChannel;

  static FSOUND_STREAM *bgm;

  static SoundUnit sounds[3];

private:
  SoundController() {/* Not supposed to be called */}
};

#endif // SOUNDCONTROLLER_H
