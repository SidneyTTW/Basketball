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

  float xFrom = MyGlobal::GYM_WIDTH / 2;
  float xTo = -MyGlobal::GYM_WIDTH / 2;
  float yFrom = -MyGlobal::GYM_LENGTH / 2;
  float yTo = MyGlobal::GYM_LENGTH / 2;

  glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    for (int i = 0;i < 100;++i)
      for (int j = 0;j < 200;++j)
      {
        glTexCoord2f(j / 200.0f, i / 100.0f);
        glVertex3f(xFrom + (xTo - xFrom) * i / 100.0f,
                   yFrom + (yTo - yFrom) * j / 200.0f,
                   0.0f);
        glTexCoord2f((j + 1) / 200.0f, i / 100.0f);
        glVertex3f(xFrom + (xTo - xFrom) * i / 100.0f,
                   yFrom + (yTo - yFrom) * (j + 1) / 200.0f,
                   0.0f);
        glTexCoord2f((j + 1) / 200.0f, (i + 1) / 100.0f);
        glVertex3f(xFrom + (xTo - xFrom) * (i + 1) / 100.0f,
                   yFrom + (yTo - yFrom) * (j + 1) / 200.0f,
                   0.0f);
        glTexCoord2f(j / 200.0f, (i + 1) / 100.0f);
        glVertex3f(xFrom + (xTo - xFrom) * (i + 1) / 100.0f,
                   yFrom + (yTo - yFrom) * j / 200.0f,
                   0.0f);
      }
  glEnd();

  glPopMatrix();
}

void Floor::renderMirror()
{
  glPushMatrix();
  glTranslatef(translate._x, translate._y, translate._z);

  glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(MyGlobal::GYM_WIDTH / 2,
               -MyGlobal::GYM_LENGTH / 2,
               0.0f);
    glVertex3f(MyGlobal::GYM_WIDTH / 2,
               MyGlobal::GYM_LENGTH / 2,
               0.0f);
    glVertex3f(-MyGlobal::GYM_WIDTH / 2,
               MyGlobal::GYM_LENGTH / 2,
               0.0f);
    glVertex3f(-MyGlobal::GYM_WIDTH / 2,
               -MyGlobal::GYM_LENGTH / 2,
               0.0f);
  glEnd();

  glPopMatrix();
}
