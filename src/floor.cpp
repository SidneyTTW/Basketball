#include "floor.h"

#include "glaid.h"
#include "myglobal.h"

int Floor::texture = -1;

Floor::Floor() :
    Flat(MyGlobal::GYM_WIDTH / 2, MyGlobal::GYM_LENGTH / 2)
{
  if (texture == -1)
    texture = GLAid::loadTexture(MyGlobal::COURT_TEXTURE_PATH);
}

void Floor::render()
{
  glPushMatrix();
  glTranslatef(translate._x, translate._y, translate._z);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);

  glColor3f(0.8, 0.8, 0.8);

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
