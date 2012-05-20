#include "soundcontroller.h"

#include <fmod_errors.h>
#include "myglobal.h"
#include <QDebug>

Point3D SoundController::listennerPos = Point3D();

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

  // Load sounds/musics
  for (int i = 0;i < 3;++i)
  {
    sounds[i].channel = -1;
    sounds[i].sound = FSOUND_Sample_Load(FSOUND_UNMANAGED,
                                         MyGlobal::SOUND_PATHS[i],
                                         FSOUND_HW2D,
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
  FSOUND_SetVolume(channel, 10);
  FSOUND_SetPaused(channel, false);

  footstep = FSOUND_Sample_Load(FSOUND_UNMANAGED,
                                MyGlobal::SOUND_PATHS[4],
                                FSOUND_LOOP_NORMAL,
                                0,
                                0);
  footstepChannel = FSOUND_PlaySoundEx(FSOUND_FREE, footstep, NULL, true);
  FSOUND_SetVolume(footstepChannel, 250);
  FSOUND_SetPaused(footstepChannel, true);
}

void SoundController::playSound
    (BuiltInSoundType type, Point3D pos, double volumn)
{
  if (volumn <= 0.05)
    return;

//  float pos2[3];
//  pos2[0] = pos._x;
//  pos2[1] = pos._y;
//  pos2[2] = pos._z;

//  float vel[3] = { 0,0,0 };

  if (sounds[type].channel != -1)
    FSOUND_StopSound(sounds[type].channel);

  sounds[type].channel = FSOUND_PlaySoundEx(FSOUND_FREE, sounds[type].sound, NULL, true);
  double dis = (listennerPos - pos).radius();
//  FSOUND_3D_SetAttributes(sounds[type].channel, pos2, vel);
  if (dis < 1)
    FSOUND_SetVolume(sounds[type].channel, 255 * volumn);
  else
    FSOUND_SetVolume(sounds[type].channel, 255 * volumn / ((dis + 9) / 5));

  FSOUND_SetPaused(sounds[type].channel, false);
  sounds[type].position = pos;
  sounds[type].volumn = volumn;
}

void SoundController::setListennerPosition(Point3D position, Point3D direction)
{
  return;
  listennerPos = position;
  double tp, ts, tr;
  double dis;
  float pos[3] = { 0,0,0 };
  float vel[3] = { 0,0,0 };
  for (int i = 0;i < 3;++i)
  {
    if (sounds[i].channel != -1)
    {
      pos[0] = position._x;
      pos[1] = position._y;
      pos[2] = position._z;
      vel[0] = direction._x;
      vel[1] = direction._y;
      vel[2] = direction._z;
      FSOUND_3D_Listener_SetAttributes(pos, vel, 0, 0, 1.0f, 0, 1.0f, 0);
      ts = calculateAngle(direction._x, direction._y);
      direction = sounds[i].position - listennerPos;
      tp = calculateAngle(direction._x, direction._y);
      dis = qSqrt(direction._x * direction._x + direction._y * direction._y);
      tr = tp - ts;
      vel[0] = -qCos(tr) * dis;
      FSOUND_3D_SetAttributes(sounds[i].channel, pos, vel);
    }
  }
}
