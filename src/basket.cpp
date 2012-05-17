#include "basket.h"

#include <QtOpenGL>
#include "3ds.h"
#include "flat.h"
#include "myglobal.h"
#include "myworld.h"
#include "ring.h"

C3DSModel *Basket::model = NULL;

Basket::Basket(Point3D translate, double t, MyWorld *world) :
    _translate(translate),
    _t(t)
{
  if (!model)
  {
    model = new C3DSModel();
    model->Load(MyGlobal::BASKET_3DS_PATH);
  }

  rebound = new Flat(MyGlobal::REBOUND_WIDTH / 2,
                     MyGlobal::REBOUND_HEIGHT / 2);
  rebound->translate = translate;
  double yDirection = qCos(t);
  rebound->translate += Point3D(0,
                                0.4 * yDirection,
                                MyGlobal::REBOUND_BOTTOM +
                                MyGlobal::REBOUND_HEIGHT / 2 - 0.07);
  world->addStaticXZFlat(rebound);

  ring = new Ring(MyGlobal::BASKET_INNER_RADIUS + 0.08,
                  MyGlobal::BASKET_OUTER_RADIUS);
  ring->translate = translate;
  ring->translate._z += MyGlobal::BASKET_HEIGHT;
  world->addStaticXYRing(ring);
}

void Basket::render()
{
  glPushMatrix();

  glTranslatef(_translate._x, _translate._y, _translate._z);
  glRotatef(_t * 180 / PI, 0, 0, 1);

  glTranslatef(0, 0.45, 1.15);

  glScalef(0.57, 0.57, 0.57);

  model->Render();

  glPopMatrix();

//  glDisable(GL_TEXTURE_2D);
//  glColor3f(1, 0, 0);
//  glBegin(GL_QUADS);
//  glVertex3f(rebound->translate._x - rebound->size1,
//             rebound->translate._y,
//             rebound->translate._z + rebound->size2);
//  glVertex3f(rebound->translate._x + rebound->size1,
//             rebound->translate._y,
//             rebound->translate._z + rebound->size2);
//  glVertex3f(rebound->translate._x + rebound->size1,
//             rebound->translate._y,
//             rebound->translate._z - rebound->size2);
//  glVertex3f(rebound->translate._x - rebound->size1,
//             rebound->translate._y,
//             rebound->translate._z - rebound->size2);
//  glEnd();

//  GLUquadric* quadric = gluNewQuadric();
//  gluQuadricNormals(quadric, GL_SMOOTH);
//  gluQuadricTexture(quadric, GL_TRUE);

//  glTranslatef( ring->translate._x,  ring->translate._y, ring->translate._z);
//  gluSphere(quadric, ring->R, 32, 32);

//  gluDeleteQuadric(quadric);
}

Basket::~Basket()
{
  delete ring;
  delete rebound;
}
