#include "basket.h"

#include <QtOpenGL>
#include "3ds.h"
#include "flat.h"
#include "myglobal.h"
#include "myworld.h"
#include "ring.h"

float Basket::materialAmbient[4] = {0.2, 0.2, 0.2, 1.0};
float Basket::materialDiffuse[4] = {1.0, 1.0, 1.0, 1.0};
float Basket::materialSpecular[4] = {0.0, 0.0, 0.0, 1.0};

C3DSModel *Basket::model = NULL;

Basket::Basket(Point3D translate, double t, MyWorld *world) :
    _translate(translate),
    _t(t)
{
  glEnable(GL_COLOR_MATERIAL);
  if (!model)
  {
    model = new C3DSModel();
    model->Load(MyGlobal::BASKET_3DS_PATH);
  }

  rebound = new Flat(MyGlobal::REBOUND_WIDTH / 2 - 0.03,
                     MyGlobal::REBOUND_HEIGHT / 2 - 0.02);
  rebound->translate = translate;
  double yDirection = qCos(t);
  rebound->translate += Point3D(0,
                                0.41 * yDirection,
                                MyGlobal::REBOUND_BOTTOM +
                                MyGlobal::REBOUND_HEIGHT / 2 - 0.08);
  world->addStaticXZFlat(rebound);

  ring = new Ring(MyGlobal::BASKET_INNER_RADIUS + 0.08,
                  MyGlobal::BASKET_OUTER_RADIUS);
  ring->translate = translate;
  ring->translate._z += MyGlobal::BASKET_HEIGHT;
  world->addStaticXYRing(ring);
}

void Basket::render()
{
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
  glPushMatrix();

  glTranslatef(_translate._x, _translate._y, _translate._z);
  glRotatef(_t * 180 / PI, 0, 0, 1);

  glTranslatef(0, 0.45, 1.15);

  glScalef(0.57, 0.57, 0.57);

  model->Render();

  glPopMatrix();

//  GLUquadric* quadric = gluNewQuadric();
//  gluQuadricNormals(quadric, GL_SMOOTH);
//  gluQuadricTexture(quadric, GL_TRUE);

//  glTranslatef( ring->translate._x,  ring->translate._y, ring->translate._z);
//  gluSphere(quadric, ring->R, 32, 32);

//  gluDeleteQuadric(quadric);
}

void Basket::renderRebound()
{
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
  glDisable(GL_TEXTURE_2D);
  glPushMatrix();
  glRotatef(_t * 180 / PI, 0, 0, 1);
  glNormal3f(0, -1, 0);
  glPopMatrix();
  glColor4f(1, 1, 1, 0.3);
  glBegin(GL_QUADS);
  glVertex3f(rebound->translate._x - rebound->size1,
             rebound->translate._y,
             rebound->translate._z + rebound->size2);
  glVertex3f(rebound->translate._x + rebound->size1,
             rebound->translate._y,
             rebound->translate._z + rebound->size2);
  glVertex3f(rebound->translate._x + rebound->size1,
             rebound->translate._y,
             rebound->translate._z - rebound->size2);
  glVertex3f(rebound->translate._x - rebound->size1,
             rebound->translate._y,
             rebound->translate._z - rebound->size2);
  glEnd();
}

Basket::~Basket()
{
  delete ring;
  delete rebound;
}
