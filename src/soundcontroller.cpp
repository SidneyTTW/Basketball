#include "soundcontroller.h"

#include <fmod_errors.h>
#include "myglobal.h"

SoundController::SoundUnit SoundController::sounds[3];

FSOUND_STREAM *SoundController::bgm = NULL;

FSOUND_SAMPLE *SoundController::footstep = NULL;

int SoundController::footstepChannel = -1;

void SoundController::init()
{
  if (FSOUND_GetVersion() < FMOD_VERSION)
  {
      //printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
      return;
  }

  FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);

  if (!FSOUND_Init(44100, 32, 0))
    return;
  float pos[] = {0.0f, 0.0f, 0.0f};
  FSOUND_3D_Listener_SetAttributes(pos, pos, 0, 0, 1.0f, 0, 1.0f, 0);

  // Load sounds/musics
  for (int i = 0;i < 3;++i)
  {
    sounds[i].channel = -1;
    sounds[i].sound = FSOUND_Sample_Load(FSOUND_FREE,
                                         MyGlobal::SOUND_PATHS[i],
                                         FSOUND_HW3D,
                                         0,
                                         0);
    FSOUND_Sample_SetMinMaxDistance(sounds[i].sound, 1.0f, 20.0f);
    sounds[i].position = Point3D();
    sounds[i].volumn = 1;
  }

  bgm = FSOUND_Stream_Open(MyGlobal::SOUND_PATHS[3],
                           FSOUND_LOOP_NORMAL,
                           0,
                           0);
  int channel = FSOUND_Stream_PlayEx(FSOUND_FREE, bgm, NULL, true);
  FSOUND_SetVolume(channel, 4);
  FSOUND_SetPaused(channel, false);

  footstep = FSOUND_Sample_Load(FSOUND_UNMANAGED,
                                MyGlobal::SOUND_PATHS[4],
                                FSOUND_LOOP_NORMAL,
                                0,
                                0);
  footstepChannel = FSOUND_PlaySoundEx(FSOUND_FREE, footstep, NULL, true);
  FSOUND_SetVolume(footstepChannel, 30);
  FSOUND_SetPaused(footstepChannel, true);
}

void SoundController::playSound
    (BuiltInSoundType type, Point3D pos, double volumn)
{
  if (volumn <= 0.03 && volumn != OpenShot)
    return;

  float zero[3] = {0.0f, 0.0f, 0.0f};

  if (sounds[type].channel != -1)
    FSOUND_StopSound(sounds[type].channel);

  sounds[type].channel = FSOUND_PlaySoundEx(FSOUND_FREE, sounds[type].sound, NULL, true);
  FSOUND_SetVolume(sounds[type].channel, 0);
  FSOUND_3D_SetAttributes(sounds[type].channel, zero, zero);
  sounds[type].position = pos;
  sounds[type].volumn = volumn;
}

void SoundController::setListennerPosition(Point3D position, Point3D direction)
{
  double tp, ts, tr;
  double dis;
  float pos[3] = {0, 0, 0};
  float vel[3] = {0, 0, 0};
  for (int i = 0;i < 3;++i)
  {
    if (sounds[i].channel != -1)
    {
      ts = calculateAngle(direction._x, direction._y);
      direction = sounds[i].position - position;
      tp = calculateAngle(direction._x, direction._y);
      dis = qSqrt(direction._x * direction._x + direction._y * direction._y);
      tr = ts - tp;
      pos[0] = qSin(tr) * (dis >= 3 ? 3.0f : dis);
      FSOUND_3D_SetAttributes(sounds[i].channel, pos, vel);
      if (dis < 1)
        FSOUND_SetVolume(sounds[i].channel, 255 * sounds[i].volumn);
      else
        FSOUND_SetVolume(sounds[i].channel, 255 * sounds[i].volumn / ((dis + 14) / 15));
      FSOUND_SetPaused(sounds[i].channel, false);
    }
  }
}
