#include "basketball.h"

#include <GL/glext.h>
#include <qmath.h>
#include "glaid.h"
#include "myglobal.h"

int BasketBall::texture = -1;

float BasketBall::materialAmbient[4] = {0.5, 0.5, 0.5, 1.0};
float BasketBall::materialDiffuse[4] = {0.8, 0.8, 0.8, 1.0};
float BasketBall::materialSpecular[4] = {0.5, 0.5, 0.5, 1.0};
float BasketBall::materialShininess[1] = {10.0};
float BasketBall::originalShininess[1] = {1000.0};

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
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
  glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

  glPushMatrix();
  glTranslatef(translate._x, translate._y, translate._z);

  glMultMatrixf(matrix);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);

  glColor3ub(180, 120, 0);
  gluSphere(quadric, MyGlobal::BASKETBALL_RADIUS, 32, 32);

  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  glMaterialfv(GL_FRONT, GL_SHININESS, originalShininess);
}

BasketBall::~BasketBall()
{
  gluDeleteQuadric(quadric);
}
