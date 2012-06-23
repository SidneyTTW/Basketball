#include "stadium.h"

#include <QtOpenGL>
#include "3ds.h"
#include "myglobal.h"

float Stadium::materialAmbient[4] = {0.8, 0.8, 0.8, 1.0};
float Stadium::materialDiffuse[4] = {0.8, 0.8, 0.8, 1.0};
float Stadium::materialSpecular[4] = {0.2, 0.2, 0.2, 1.0};

C3DSModel *Stadium::model = NULL;

Stadium::Stadium(Point3D translate) : _translate(translate)
{
  if (!model)
  {
    model = new C3DSModel();
    model->Load(MyGlobal::STADIUM_3DS_PATH);
  }
}

void Stadium::render()
{
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
  glPushMatrix();

  glTranslatef(_translate._x, _translate._y, _translate._z);

  model->Render();

  glPopMatrix();
}
