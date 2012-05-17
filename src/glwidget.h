#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include <QSet>
#include <QTimer>
#include "3ds.h"
#include "point3d.h"

class AbstractCamera;
class Ball;
class Basket;
class BasketBall;
class Flat;
class Floor;
class MyWorld;
class Net;
class Ring;

/**
 * Class of main widget.
 */
class GLWidget : public QGLWidget
{
  Q_OBJECT
public:
  /**
   * Constructor.
   */
  GLWidget();

  /**
   * Destructor.
   */
  virtual ~GLWidget();

  /**
   * Type of the camera.
   */
  enum CameraType{TypeAudience=0, TypePlayer, TypeZ};

protected:
  virtual void initializeGL();
  virtual void paintGL();
  virtual void resizeGL(int width, int height);
  virtual void keyPressEvent(QKeyEvent *event);
  virtual void keyReleaseEvent(QKeyEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);

private:
  /**
   * A timer used to advance.
   */
  QTimer *timer;

  /**
   * The basketball.
   */
  BasketBall *basketBall;

  /**
   * The floor.
   */
  Floor *floor;

  /**
   * The walls.
   */
  Flat *walls[4];

  /**
   * The world.
   */
  MyWorld *world;

  /**
   * The baskets.
   */
  Basket *basket[2];

  /**
   * The nets.
   */
  Net *net[2];

  /**
   * The cameras.
   */
  AbstractCamera *cameras[3];

  /**
   * Type of the camera.
   */
  CameraType cameraType;

  /**
   * The keys pressed.
   */
  QSet<int> pressed;

  /**
   * The speed of the game(default is 20).
   */
  int speed;

  /**
   * Last position of left mouse button.
   */
  QPoint lastPosition;

  /**
   * Whether left mouse if pressed.
   */
  bool mousePressed;

  /**
   * Whether a shot may be an open one.
   */
  bool mayBeOpenShot;

  /**
   * The angle to calculate the force to throw the basketball.
   */
  double forceAngle;

  /**
   * The positions built in.
   */
  static Point3D builtInPositions[15];

  /**
   * The speeds built in.
   */
  static Point3D builtInSpeeds[15];

  /**
   * Whether to draw the mirror part.
   */
  bool mirror;

  /**
   * @return The speed of the ball if user throws it with current power.
   */
  double throwSpeed();

  /**
   * Draw the power of the player.
   */
  void paintPower();

private slots:
  /**
   * A slot called to advance.
   */
  void advance();

  void bounceBallFlat(Ball *ball, Flat *flat);
  void beforeBounceBallRing(Ball *ball, Ring *ring);
  void score(Ball *ball, Ring *ring, bool openShot);
};

#endif // GLWIDGET_H
