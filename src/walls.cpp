#include "walls.h"

#include "glaid.h"
#include "flat.h"
#include "myglobal.h"
#include "myworld.h"

int * Walls::textures = NULL;

float Walls::materialAmbient[4] = {0.2, 0.2, 0.2, 1.0};
float Walls::materialDiffuse[4] = {1.0, 1.0, 1.0, 1.0};
float Walls::materialSpecular[4] = {0.0, 0.0, 0.0, 1.0};

#define NEAR_RATE    1.8
#define FAR_RATE     4
#define TOP          15.0

Walls::Walls(MyWorld *world)
{
  walls[0] = new Flat(MyGlobal::GYM_WIDTH, 2000);
  walls[0]->translate = Point3D(0, MyGlobal::GYM_LENGTH / 2, 0);
  walls[1] = new Flat(MyGlobal::GYM_WIDTH, 2000);
  walls[1]->translate = Point3D(0, -MyGlobal::GYM_LENGTH / 2, 0);
  walls[2] = new Flat(MyGlobal::GYM_LENGTH, 2000);
  walls[2]->translate = Point3D(MyGlobal::GYM_WIDTH / 2, 0, 0);
  walls[3] = new Flat(MyGlobal::GYM_LENGTH, 2000);
  walls[3]->translate = Point3D(-MyGlobal::GYM_WIDTH / 2, 0, 0);
  world->addStaticXZFlat(walls[0]);
  world->addStaticXZFlat(walls[1]);
  world->addStaticYZFlat(walls[2]);
  world->addStaticYZFlat(walls[3]);
  if (textures == NULL)
  {
    textures = new int[4];
    textures[0] = GLAid::loadTexture(MyGlobal::WALLS_TEXTURE_PATH[0]);
    textures[1] = GLAid::loadTexture(MyGlobal::WALLS_TEXTURE_PATH[1]);
    textures[2] = GLAid::loadTexture(MyGlobal::WALLS_TEXTURE_PATH[2]);
    textures[3] = GLAid::loadTexture(MyGlobal::WALLS_TEXTURE_PATH[3]);
  }
}

void Walls::render()
{
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

  glColor3ub(200, 200, 200);

  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, textures[0]);

  glBegin(GL_QUADS);
    glNormal3f(0.0, -1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-MyGlobal::GYM_WIDTH / 2 * NEAR_RATE,
                                         MyGlobal::GYM_LENGTH / 2 * NEAR_RATE,
                                         0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(MyGlobal::GYM_WIDTH / 2 * NEAR_RATE,
                                         MyGlobal::GYM_LENGTH / 2 * NEAR_RATE,
                                         0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(MyGlobal::GYM_WIDTH / 2 * NEAR_RATE,
                                         MyGlobal::GYM_LENGTH / 2 * FAR_RATE,
                                         TOP);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-MyGlobal::GYM_WIDTH / 2 * NEAR_RATE,
                                         MyGlobal::GYM_LENGTH / 2 * FAR_RATE,
                                         TOP);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, textures[1]);

  glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(MyGlobal::GYM_WIDTH / 2 * NEAR_RATE,
                                         -MyGlobal::GYM_LENGTH / 2 * NEAR_RATE,
                                         0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-MyGlobal::GYM_WIDTH / 2 * NEAR_RATE,
                                         -MyGlobal::GYM_LENGTH / 2 * NEAR_RATE,
                                         0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-MyGlobal::GYM_WIDTH / 2 * NEAR_RATE,
                                         -MyGlobal::GYM_LENGTH / 2 * FAR_RATE,
                                         TOP);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(MyGlobal::GYM_WIDTH / 2 * NEAR_RATE,
                                         -MyGlobal::GYM_LENGTH / 2 * FAR_RATE,
                                         TOP);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, textures[2]);

  glBegin(GL_QUADS);
    glNormal3f(-1.0, 0.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(MyGlobal::GYM_WIDTH / 2 * NEAR_RATE,
                                         MyGlobal::GYM_LENGTH / 2 * NEAR_RATE,
                                         0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(MyGlobal::GYM_WIDTH / 2 * NEAR_RATE,
                                         -MyGlobal::GYM_LENGTH / 2 * NEAR_RATE,
                                         0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(MyGlobal::GYM_WIDTH / 2 * FAR_RATE,
                                         -MyGlobal::GYM_LENGTH / 2 * NEAR_RATE,
                                         TOP);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(MyGlobal::GYM_WIDTH / 2 * FAR_RATE,
                                         MyGlobal::GYM_LENGTH / 2 * NEAR_RATE,
                                         TOP);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, textures[3]);

  glBegin(GL_QUADS);
    glNormal3f(1.0, 0.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-MyGlobal::GYM_WIDTH / 2 * NEAR_RATE,
                                         -MyGlobal::GYM_LENGTH / 2 * NEAR_RATE,
                                         0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-MyGlobal::GYM_WIDTH / 2 * NEAR_RATE,
                                         MyGlobal::GYM_LENGTH / 2 * NEAR_RATE,
                                         0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-MyGlobal::GYM_WIDTH / 2 * FAR_RATE,
                                         MyGlobal::GYM_LENGTH / 2 * NEAR_RATE,
                                         TOP);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-MyGlobal::GYM_WIDTH / 2 * FAR_RATE,
                                         -MyGlobal::GYM_LENGTH / 2 * NEAR_RATE,
                                         TOP);
  glEnd();
}

Walls::~Walls()
{
  delete walls[0];
  delete walls[1];
  delete walls[2];
  delete walls[3];
  delete [] walls;
}
