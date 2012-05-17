#ifndef GLAID_H
#define GLAID_H

#include <QtOpenGL>

namespace GLAid
{
  /**
   * Load a texture from a file.
   *
   * @param path Path of the file, can be resouce file of QT.
   * @return The id of the texture in opengl.
   */
  GLuint loadTexture(const QString& path);
}

#endif // GLAID_H
