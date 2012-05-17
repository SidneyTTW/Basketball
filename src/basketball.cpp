#include "basketball.h"

#include <qmath.h>
#include "glaid.h"
#include "myglobal.h"

int BasketBall::texture = -1;

BasketBall::BasketBall() :
    Ball(MyGlobal::BASKETBALL_RADIUS)
{
  quadric = gluNewQuadric();
  gluQuadricNormals(quadric, GL_SMOOTH);
  gluQuadricTexture(quadric, GL_TRUE);
  if (texture == -1)
    texture = GLAid::loadTexture(MyGlobal::BASKETBALL_TEXTURE_PATH);
}

void BasketBall::render()
{
  autoAdjustAngle(rotate._x);
  autoAdjustAngle(rotate._z);
  while (rotate._y < 0)
    rotate._y += PI;
  while (rotate._y > PI)
    rotate._y -= PI;

  glPushMatrix();
  glTranslatef(translate._x, translate._y, translate._z);

  glRotatef(rotate._z * 180 / PI, 0.0f, 0.0f, 1.0f);
  glRotatef(rotate._y * 180 / PI, -qSin(rotate._x - rotate._z), qCos(rotate._x - rotate._z), 0.0f);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);

    glColor3f(1.0f, 0.5f, 0.5f);
//    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, MyGlobal::BASKETBALL_RADIUS, 32, 32);

//  glColor4f(1.0, 1.0, 1.0, 0.4);
//  glEnable(GL_BLEND);
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//  glEnable(GL_TEXTURE_GEN_S);
//  glEnable(GL_TEXTURE_GEN_T);

//  gluSphere(quadric, MyGlobal::BASKETBALL_RADIUS, 32, 32);
//  glDisable(GL_TEXTURE_GEN_S);
//  glDisable(GL_TEXTURE_GEN_T);
//  glDisable(GL_BLEND);

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

BasketBall::~BasketBall()
{
  gluDeleteQuadric(quadric);
}
