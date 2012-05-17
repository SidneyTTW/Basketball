#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QtOpenGL>
#include "point3d.h"

/**
 * Abstract class of item used to paint and stimulate.
 *
 * Currently, it uses a pure function to realize the painting.
 * This supports small number of items well, but will be quite slow when using
 * large amounts of items.
 * May use static template function to call non-virtual renderDetail to meet
 * the need of both efficiency and polymorphism.
 */
class AbstractItem
{
public:
  /**
   * Paint the whole object.
   */
  inline void render()
  {
    translateAndRotate();
    renderDetail();
    antiMoveAndTranslate();
  }

  /**
   * @return The translate.
   */
  inline const Point3D& getTranslate() const
  {return _translate;}

  /**
   * @return The translate.
   */
  inline Point3D& getTranslate()
  {return _translate;}

  /**
   * Set the translate.
   *
   * @param translate The translate;
   */
  inline void setTranslate(Point3D translate)
  {_translate = translate;}

  /**
   * @return The rotate.
   */
  inline Point3D& getRotate()
  {return _rotate;}

  /**
   * @return The rotate.
   */
  inline const Point3D& getRotate() const
  {return _rotate;}

  /**
   * Set the rotate.
   *
   * @param rotate The rotate;
   */
  inline void setRotate(Point3D rotate)
  {_rotate = rotate;}

  /**
   * Translate the item.
   *
   * @param translate How much to translate;
   */
  inline void translate(const Point3D& translate)
  {_translate += translate;}

  /**
   * Rotate the item.
   *
   * @param rotate How much to rotate;
   */
  inline void rotate(const Point3D& rotate)
  {_rotate += rotate;}

protected:
  /**
   * Paint the whole object after the matrix has been changed.
   */
  virtual void renderDetail()=0;

private:
  /**
   * The translate.
   */
  Point3D _translate;

  /**
   * The rotate.
   */
  Point3D _rotate;

  /**
   * Translate and rotate according to the _translate and _rotate.
   *
   * @warning Should call antiMoveAndTranslate after user's painting action.
   */
  inline void translateAndRotate()
  {
    glPushMatrix();
    glTranslatef(_translate._x, _translate._y, _translate._z);
    glRotatef(_rotate._x, 1.0f, 0.0f, 0.0f);
    glRotatef(_rotate._y, 0.0f, 1.0f, 0.0f);
    glRotatef(_rotate._z, 0.0f, 0.0f, 1.0f);
  }

  /**
   * Translate and rotate back according to the _translate and _rotate.
   *
   * @warning Should be called after user's painting action.
   */
  inline void antiMoveAndTranslate()
  {glPopMatrix();}

};

#endif // ABSTRACTITEM_H
