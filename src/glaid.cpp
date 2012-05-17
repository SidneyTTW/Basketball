#include "glaid.h"

GLuint GLAid::loadTexture(const QString& path)
{
  static int index = 0;
  ++index;
  GLuint result = index;
  QImage tex, buf;
  if(!buf.load(path))
  {
      QImage dummy(128, 128, QImage::Format_RGB32);
      dummy.fill(12);
      buf = dummy;
  }
  tex = QGLWidget::convertToGLFormat(buf);
  glGenTextures(1, &result);
  glBindTexture(GL_TEXTURE_2D, result);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, tex.width(), tex.height(), 0,
               GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  return result;
}
