#include "audience.h"

#include "glaid.h"
#include "myglobal.h"

int * Audience::textures = NULL;

float Audience::materialAmbient[4] = {0.2, 0.2, 0.2, 1.0};
float Audience::materialDiffuse[4] = {1.0, 1.0, 1.0, 1.0};
float Audience::materialSpecular[4] = {0.0, 0.0, 0.0, 1.0};

Audience::Audience(Point3D translate, double t) :
    _translate(translate),
    _t(t)
{
  id = qrand() % 4;
  if (textures == NULL)
  {
    textures = new int[4];
    textures[0] = GLAid::loadTexture(MyGlobal::AUDIENCE_TEXTURE_PATH[0]);
    textures[1] = GLAid::loadTexture(MyGlobal::AUDIENCE_TEXTURE_PATH[1]);
    textures[2] = GLAid::loadTexture(MyGlobal::AUDIENCE_TEXTURE_PATH[2]);
    textures[3] = GLAid::loadTexture(MyGlobal::AUDIENCE_TEXTURE_PATH[3]);
  }
}

void Audience::render()
{
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

  glColor3ub(200, 200, 200);

  glPushMatrix();

  glTranslated(_translate._x, _translate._y, _translate._z);
  glRotated(_t * 180 / PI, 0.0, 0.0, 1.0);
  glRotated(90, 1.0, 0.0, 0.0);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textures[id]);

  glBegin(GL_QUADS);
    glNormal3f(0.0, -1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.3f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3f, 1.2f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3f, 1.2f, 0.0f);
  glEnd();

  glPopMatrix();
}

