#ifndef ABSTRACTCAMERA_H
#define ABSTRACTCAMERA_H

#include <point3d.h>

/**
 * Abstract class of camera, used to call gluLookAt with correct parameter.
 */
class AbstractCamera
{
public:
  /**
   * Function to call gluLookAt.
   */
  virtual void look() const=0;

  /**
   * Function to get the from position of gluLookAt.
   */
  virtual Point3D fromPos() const=0;

  /**
   * Function to get the to position of gluLookAt.
   */
  virtual Point3D toPos() const=0;
};

#endif // ABSTRACTCAMERA_H
