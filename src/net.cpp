#include "net.h"

#include "myglobal.h"

#define MAX_ANIM          700
#define HALF_WAY          200
#define STRANGLE_OFFSET_Y (-2.73)
#define STRANGLE_OFFSET_Z 1.88
#define NET_TOP           (-STRANGLE_OFFSET_Z)
#define NET_BOTTOM        (-0.746 - STRANGLE_OFFSET_Z)

float Net::materialAmbient[4] = {0.2, 0.2, 0.2, 1.0};
float Net::materialDiffuse[4] = {1.0, 1.0, 1.0, 1.0};
float Net::materialSpecular[4] = {0.0, 0.0, 0.0, 1.0};

Net::Net(Point3D translate) :
    anim(-1),
    _translate(translate),
    _t(0),
    maxOffset(0),
    _rotate(0)
{
  Load(MyGlobal::NET_3DS_PATH);
}

void Net::render()
{
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
  glPushMatrix();
  glTranslatef(_translate._x, _translate._y, _translate._z);
  glRotatef(_rotate * 180 / PI, 0, 0, 1);
  glScalef(0.57, 0.57, 0.57);
  glTranslatef(0, -2.73, 1.87);
  if (anim == -1)
  {
    C3DSModel::Render();
    glPopMatrix();
    return;
  }
  double offset = anim <= HALF_WAY ?
                  anim * maxOffset / HALF_WAY :
                  (MAX_ANIM - anim) * maxOffset / (MAX_ANIM - HALF_WAY);
  double xOffset = offset * qCos(_t - _rotate);
  double yOffset = offset * qSin(_t - _rotate);
  Vector3 **vertsBackup = new Vector3 *[m_3DModel.numOfObjects];
  for (int i = 0;i < m_3DModel.numOfObjects;++i)
  {
    vertsBackup[i] = m_3DModel.pObject[i].pVerts;
    m_3DModel.pObject[i].pVerts = new Vector3[m_3DModel.pObject[i].numOfVerts];
    for (int j = 0;j < m_3DModel.pObject[i].numOfVerts;++j)
    {
      m_3DModel.pObject[i].pVerts[j] = vertsBackup[i][j];
      if (m_3DModel.pObject[i].pVerts[j].z < NET_TOP)
      {
        double rate = (NET_TOP - m_3DModel.pObject[i].pVerts[j].z) *
                      (NET_TOP - m_3DModel.pObject[i].pVerts[j].z) /
                      (NET_TOP - NET_BOTTOM);
        m_3DModel.pObject[i].pVerts[j].x += xOffset * rate;
        m_3DModel.pObject[i].pVerts[j].y += yOffset * rate;
      }
    }
  }
  C3DSModel::Render();
  for (int i = 0;i < m_3DModel.numOfObjects;++i)
  {
    delete [] m_3DModel.pObject[i].pVerts;
    m_3DModel.pObject[i].pVerts = vertsBackup[i];
  }
  delete [] vertsBackup;
  glPopMatrix();
}

void Net::advance()
{
  if (anim == -1)
    return;
  ++anim;
  if (anim > MAX_ANIM)
    anim = -1;
}
