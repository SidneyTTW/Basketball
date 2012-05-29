#include "floor.h"

#include "glaid.h"
#include "myglobal.h"

int Floor::texture = -1;

float Floor::materialAmbient[4] = {0.2, 0.2, 0.2, 1.0};
float Floor::materialDiffuse[4] = {1.0, 1.0, 1.0, 1.0};
float Floor::materialSpecular[4] = {0.0, 0.0, 0.0, 1.0};

Floor::Floor() :
    Flat(MyGlobal::GYM_WIDTH / 2, MyGlobal::GYM_LENGTH / 2)
{
  if (texture == -1)
    texture = GLAid::loadTexture(MyGlobal::COURT_TEXTURE_PATH);
}

void Floor::render()
{
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
  glPushMatrix();
  glTranslatef(translate._x, translate._y, translate._z);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);

  glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(MyGlobal::GYM_WIDTH / 2,
                                         -MyGlobal::GYM_LENGTH / 2,
                                         0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(MyGlobal::GYM_WIDTH / 2,
                                         MyGlobal::GYM_LENGTH / 2,
                                         0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-MyGlobal::GYM_WIDTH / 2,
                                         MyGlobal::GYM_LENGTH / 2,
                                         0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-MyGlobal::GYM_WIDTH / 2,
                                         -MyGlobal::GYM_LENGTH / 2,
                                         0.0f);
  glEnd();

  glPopMatrix();
}
