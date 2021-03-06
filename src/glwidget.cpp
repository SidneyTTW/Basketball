#include "glwidget.h"

#include <QtOpenGL>
#include <QKeyEvent>
#include <qmath.h>
#include "audience.h"
#include "audiencecamera.h"
#include "basket.h"
#include "basketball.h"
#include "floor.h"
#include "glaid.h"
#include "myglobal.h"
#include "myworld.h"
#include "net.h"
#include "playercamera.h"
#include "ring.h"
#include "soundcontroller.h"
#include "stadium.h"
#include "walls.h"
#include "zcamera.h"

Point3D GLWidget::builtInPositions[15] =
{
  Point3D(0, 8, 1.7),
  Point3D(0, 8, 1.7),
  Point3D(0, 8, 1.7),
  Point3D(0, 8, 1.7),
  Point3D(0, 8, 1.7),
  Point3D(0, 8, 1.7),
  Point3D(0, 8, 1.7),
  Point3D(0, 8, 1.7),
  Point3D(0, 8, 1.7),
  Point3D(0, 8, 1.7),
  Point3D(0, 8, 1.7),
  Point3D(3.5, 9.3, 1.7),
  Point3D(3.5, 9.3, 1.7),
  Point3D(3.5, 9.3, 1.7),
  Point3D(3.5, 9.3, 1.7)
};

Point3D GLWidget::builtInSpeeds[15] =
{
  Point3D(0, 3.0, 6.5),
  Point3D(0, 3.1, 6.5),
  Point3D(0, 3.2, 6.5),
  Point3D(0, 3.3, 6.5),
  Point3D(0, 3.4, 6.5),
  Point3D(0, 3.5, 6.5),
  Point3D(0, 3.6, 6.5),
  Point3D(0, 3.7, 6.5),
  Point3D(0, 3.8, 6.5),
  Point3D(0, 3.9, 6.5),
  Point3D(0, 4.0, 6.5),
  Point3D(-3.28, 2.28, 6.5),
  Point3D(-3.35, 2.35, 6.5),
  Point3D(-3.43, 2.43, 6.5),
  Point3D(-3.5, 2.5, 6.5)
};

GLfloat noLight[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat ambient[] = {0.25f, 0.25, 0.25, 1.0f};
GLfloat diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat position[] = {0.0f, 0.0f, 10.0f, 1.0f};
GLfloat spotDir[] = {0.0f, 0.0f, -10.0f};
GLfloat fogColor[] = {0.2f, 0.2f, 0.2f, 1.0f};

GLWidget::GLWidget() :
    cameraType(TypeAudience),
    speed(40),
    forceAngle(0),
    mouseMoving(false),
    mousePressed(false),
    mayBeOpenShot(true),
    mirror0(true),
    mirror1(false),
    mirrorFloor(true)
{
  setWindowIcon(QIcon(MyGlobal::APP_ICON_PATH));
  setMouseTracking(true);
  timer = new QTimer();
  timer->setInterval(40);
  connect(timer, SIGNAL(timeout()), SLOT(advance()));
  timer->start();
}

void GLWidget::initializeGL()
{
  basketBall = new BasketBall();
  basketBall->translate = Point3D(0, 0, 0.123);
  basketBall->speed = Point3D();
  basketBall->rotateSpeed = Point3D();

  floor = new Floor();

  world  = new MyWorld();
  world->addDynamicBall(basketBall);
  world->addStaticXYFlat(floor);
  walls = new Walls(world);

  stadium = new Stadium(Point3D(0, 0, 0));

  basket[0] = new Basket(Point3D(0, MyGlobal::BASKETBALL_COURT_LENGTH / 2 - 2.7, 0),
                         0, world);
  basket[1] = new Basket(Point3D(0, -MyGlobal::BASKETBALL_COURT_LENGTH / 2 + 2.7, 0),
                         PI, world);

  audience[0] = new Audience(Point3D(-MyGlobal::BASKETBALL_COURT_WIDTH / 2 - 3.5,
                                     MyGlobal::BASKETBALL_COURT_LENGTH / 2 - 2.8,
                                     0.2),
                             PI / 2);
  audience[1] = new Audience(Point3D(-MyGlobal::BASKETBALL_COURT_WIDTH / 2 - 3.5,
                                     -2.1,
                                     0.2),
                             PI / 2);
  audience[2] = new Audience(Point3D(-MyGlobal::BASKETBALL_COURT_WIDTH / 2 - 3.5,
                                     MyGlobal::BASKETBALL_COURT_LENGTH / 2 - 1.65,
                                     0.2),
                             PI / 2);
  audience[3] = new Audience(Point3D(-MyGlobal::BASKETBALL_COURT_WIDTH / 2 - 3.5,
                                     0.2,
                                     0.2),
                             PI / 2);

  net[0] = new Net(Point3D(0,
                           MyGlobal::BASKETBALL_COURT_LENGTH / 2 - 2.7,
                           MyGlobal::BASKET_HEIGHT));
  net[1] = new Net(Point3D(0,
                           -MyGlobal::BASKETBALL_COURT_LENGTH / 2 + 2.7,
                           MyGlobal::BASKET_HEIGHT));
  net[1]->setRotate(PI);

  cameras[0] = new AudienceCamera(basketBall,
                                  Point3D(MyGlobal::BASKETBALL_COURT_WIDTH * 0.7,
                                          0,
                                          MyGlobal::BASKET_HEIGHT),
                                  1.5);

  cameras[1] = new PlayerCamera(Point3D(3.5, 10.5, 1.7), PI * 2 / 5, PI / 2 + PI / 3,
                                -MyGlobal::BASKETBALL_COURT_WIDTH / 2,
                                MyGlobal::BASKETBALL_COURT_WIDTH / 2,
                                -MyGlobal::BASKETBALL_COURT_LENGTH / 2 + 2.5,
                                MyGlobal::BASKETBALL_COURT_LENGTH / 2 - 2.5,
                                0.5,
                                5);
  cameras[2] = new ZCamera(basketBall, PI, 10);

  connect(world,
          SIGNAL(beforeBounceBallFlat(Ball*,Flat*)),
          SLOT(bounceBallFlat(Ball*,Flat*)));
  connect(world,
          SIGNAL(beforeBounceBallRing(Ball*,Ring*)),
          SLOT(beforeBounceBallRing(Ball*,Ring*)));
  connect(world,
          SIGNAL(ballThroughRing(Ball*,Ring*,bool)),
          SLOT(score(Ball*,Ring*,bool)));

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, ambient);
  glLightfv(GL_LIGHT0, GL_SPECULAR, noLight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHT0);

  glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
  glClearDepth(1.0f);
  glClearStencil(0);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glFogi(GL_FOG_MODE, GL_EXP);
  glFogfv(GL_FOG_COLOR, fogColor);
  glFogf(GL_FOG_DENSITY, 0.2);
  glHint(GL_FOG_HINT, GL_DONT_CARE);
}

void GLWidget::paintGL()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  cameras[(int) cameraType]->look();

  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT1, GL_POSITION, position);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0f);
  Point3D lightPos = Point3D(position[0], position[1], position[2]);
  Point3D lightDir = basketBall->translate - lightPos;
  spotDir[0] = lightDir._x;
  spotDir[1] = lightDir._y;
  spotDir[2] = lightDir._z;
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 60.0f);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT1);

  glEnable(GL_BLEND);

  stadium->render();
  walls->render();
  audience[0]->render();
  audience[1]->render();
  audience[2]->render();
  audience[3]->render();

  if (mirrorFloor)
  {
    double eqr[] = {0.0, 0.0, -1.0, 0.0};
    glColorMask(0, 0, 0, 0);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glDisable(GL_DEPTH_TEST);
    floor->renderMirror();
    glEnable(GL_DEPTH_TEST);
    glColorMask(1, 1, 1, 1);
    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, eqr);
    glDisable(GL_LIGHTING);

    glFogf(GL_FOG_START, 0);
    glFogf(GL_FOG_END, 15);
    glEnable(GL_FOG);

    glPushMatrix(); {
      glScalef(1.0, 1.0, -1.0);
      basketBall->render();
      basket[0]->render();
      basket[1]->render();
    } glPopMatrix();

    glDisable(GL_FOG);

    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_LIGHTING);
    basketBall->render();
    basket[0]->render();
    basket[1]->render();
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glColor4f(0.8, 0.8, 0.8, 0.7);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHTING);
    floor->render();
    glDisable(GL_BLEND);
    glClear(GL_STENCIL_BUFFER_BIT);
  }
  else
  {
    floor->render();
    basketBall->render();
    basket[0]->render();
    basket[1]->render();
    glDisable(GL_BLEND);
  }

  if (mirror0)
  {
    double eqr[] = {0.0, 1.0, 0.0, 0.0};
    glColorMask(0, 0, 0, 0);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glDisable(GL_DEPTH_TEST);
    basket[0]->renderRebound();
    glEnable(GL_DEPTH_TEST);
    glColorMask(1, 1, 1, 1);
    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, eqr);
    glDisable(GL_LIGHTING);

    Point3D reboundPos = basket[0]->getRebound()->translate;

    glFogf(GL_FOG_START, 0);
    glFogf(GL_FOG_END, 3);
    glEnable(GL_FOG);

    glPushMatrix(); {
      glTranslatef(0, 2 * reboundPos._y, 0);
      glScalef(1.0, -1.0, 1.0);
      if (basketBall->translate._y < reboundPos._y &&
          basketBall->translate._y + 2 > reboundPos._y)
        basketBall->render();
      net[0]->render();
    } glPopMatrix();

    glDisable(GL_FOG);

    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHTING);
    basket[0]->renderRebound();
    glDisable(GL_BLEND);
    glClear(GL_STENCIL_BUFFER_BIT);
  }
  else
    glDisable(GL_BLEND);

  if (mirror1)
  {
    double eqr[] = {0.0, -1.0, 0.0, 0.0};
    glColorMask(0, 0, 0, 0);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glDisable(GL_DEPTH_TEST);
    basket[1]->renderRebound();
    glEnable(GL_DEPTH_TEST);
    glColorMask(1, 1, 1, 1);
    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, eqr);
    glDisable(GL_LIGHTING);

    Point3D reboundPos = basket[1]->getRebound()->translate;

    glFogf(GL_FOG_START, 0);
    glFogf(GL_FOG_END, 3);
    glEnable(GL_FOG);

    glPushMatrix(); {
      glTranslatef(0, 2 * reboundPos._y, 0);
      glScalef(1.0, -1.0, 1.0);
      if (basketBall->translate._y > reboundPos._y &&
          basketBall->translate._y - 2 < reboundPos._y)
        basketBall->render();
      net[1]->render();
    } glPopMatrix();

    glDisable(GL_FOG);

    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHTING);
    basket[1]->renderRebound();
    glDisable(GL_BLEND);
    glClear(GL_STENCIL_BUFFER_BIT);
  }
  else
    glDisable(GL_BLEND);

  if (!mirror0)
    basket[0]->renderRebound();

  if (!mirror1)
    basket[1]->renderRebound();

  net[0]->render();
  net[1]->render();

  paintShadow();

  glLoadIdentity();

  paintPower();

  glFlush();
  glClear(GL_STENCIL_BUFFER_BIT);
}

void GLWidget::resizeGL(int width, int height)
{
  height = height?height:1;
  glViewport(0, 0, (GLint)width, (GLint)height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
}

void GLWidget::advance()
{
  switch (cameraType)
  {
  case TypeAudience:
    {
      if (pressed.contains(Qt::Key_Space))
        basketBall->speed._z += 0.02 * speed;
      if (pressed.contains(Qt::Key_A) || pressed.contains(Qt::Key_Left))
        basketBall->speed._y = qBound(-10.0,
                                      basketBall->speed._y - 0.02 * speed,
                                      10.0);
      if (pressed.contains(Qt::Key_D) || pressed.contains(Qt::Key_Right))
        basketBall->speed._y = qBound(-10.0,
                                      basketBall->speed._y + 0.02 * speed,
                                      10.0);
      if (pressed.contains(Qt::Key_W) || pressed.contains(Qt::Key_Up))
        basketBall->speed._x = qBound(-10.0,
                                      basketBall->speed._x - 0.02 * speed,
                                      10.0);
      if (pressed.contains(Qt::Key_S) || pressed.contains(Qt::Key_Down))
        basketBall->speed._x = qBound(-10.0,
                                      basketBall->speed._x + 0.02 * speed,
                                      10.0);
      FSOUND_SetPaused(SoundController::footstepChannel, true);
    }
    break;
  case TypePlayer:
    {
      bool moving = false;
      PlayerCamera *camera = (PlayerCamera *) (cameras[(int) cameraType]);
      if (pressed.contains(Qt::Key_A))
      {
        moving = true;
        camera->moveLeft(speed);
      }
      if (pressed.contains(Qt::Key_D))
      {
        moving = true;
        camera->moveRight(speed);
      }
      if (pressed.contains(Qt::Key_W) || pressed.contains(Qt::Key_Up))
      {
        moving = true;
        camera->moveForward(speed);
      }
      if (pressed.contains(Qt::Key_S) || pressed.contains(Qt::Key_Down))
      {
        moving = true;
        camera->moveBackward(speed);
      }
      if (pressed.contains(Qt::Key_Q))
        camera->moveDown(speed);
      if (pressed.contains(Qt::Key_E))
        camera->moveUp(speed);
      if (pressed.contains(Qt::Key_Left))
        camera->lookLeft(speed);
      if (pressed.contains(Qt::Key_Right))
        camera->lookRight(speed);
      if (pressed.contains(Qt::Key_PageUp))
        camera->lookUp(speed);
      if (pressed.contains(Qt::Key_PageDown))
        camera->lookDown(speed);
      if (mousePressed)
        forceAngle += speed * 0.001;
      Point3D from = camera->fromPos();
      if (moving &&
          from._z >= MyGlobal::MIN_PLAYER_HEIGHT &&
          from._z <= MyGlobal::MAX_PLAYER_HEIGHT)
      {
        FSOUND_SetPaused(SoundController::footstepChannel, false);
      }
      else
        FSOUND_SetPaused(SoundController::footstepChannel, true);
    }
    break;
  case TypeZ:
    {
      ZCamera *camera = (ZCamera *) (cameras[(int) cameraType]);
      if (pressed.contains(Qt::Key_A) || pressed.contains(Qt::Key_Left))
        camera->lookLeft(speed);
      if (pressed.contains(Qt::Key_D) || pressed.contains(Qt::Key_Right))
        camera->lookRight(speed);
      if (pressed.contains(Qt::Key_Space))
        basketBall->speed._z += 0.1 * speed;
      FSOUND_SetPaused(SoundController::footstepChannel, true);
    }
    break;
  }
  for (int i = 0;i < speed;++i)
  {
    world->advance();
    net[0]->advance();
    net[1]->advance();
  }
  updateGL();
  Point3D from = cameras[(int) cameraType]->fromPos();
  Point3D to = cameras[(int) cameraType]->toPos();
  SoundController::setListennerPosition(from, to - from);
}

void GLWidget::bounceBallFlat(Ball *ball, Flat *flat)
{
  double v = qSqrt(ball->speed._x * ball->speed._x +
                   ball->speed._y * ball->speed._y +
                   ball->speed._z * ball->speed._z);
  if (flat == floor)
  {
//    qDebug() << "bounce ground";
    v = qAbs(ball->speed._z);
  }
  else if (flat == basket[0]->getRebound() || flat == basket[1]->getRebound())
  {
//    qDebug() << "bounce rebound";
    v = qAbs(ball->speed._y);
  }
  else if (flat == walls->walls[0] || flat == walls->walls[1])
  {
//    qDebug() << "bounce wall";
    v = qAbs(ball->speed._y);
  }
  else if (flat == walls->walls[2] || flat == walls->walls[3])
  {
//    qDebug() << "bounce wall";
    v = qAbs(ball->speed._x);
  }
//  else
//    qDebug() << "bounce other";
  double volumn = qBound(0.0, v / 5, 0.8);
  SoundController::playSound(SoundController::BounceFlat, ball->translate, volumn);
  mayBeOpenShot = false;
}

void GLWidget::beforeBounceBallRing(Ball *ball, Ring *ring)
{
//  qDebug() << "bounce ring";
  double v = qSqrt(ball->speed._x * ball->speed._x +
                   ball->speed._y * ball->speed._y +
                   ball->speed._z * ball->speed._z);
  double volumn = qBound(0.0, v / 10, 1.0);
  SoundController::playSound(SoundController::BounceBasket, ball->translate, volumn);
  mayBeOpenShot = false;
}

void GLWidget::score(Ball *ball, Ring *ring, bool openShot)
{
// I'm sorry that the "openShot" parameter is a fake one-.-
  double v = qSqrt(ball->speed._x * ball->speed._x +
                   ball->speed._y * ball->speed._y +
                   ball->speed._z * ball->speed._z);
  double volumn = qBound(0.0, v / 5, 1.0);
  if (ring == basket[0]->getRing())
    net[0]->setOffset(calculateAngle(ball->speed._x, ball->speed._y),
                      v / 10);
  if (ring == basket[1]->getRing())
    net[1]->setOffset(calculateAngle(ball->speed._x, ball->speed._y),
                      v / 10);
  SoundController::playSound(SoundController::OpenShot, ball->translate,
                             mayBeOpenShot ? volumn : (volumn / 2));
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
  int builtInOperation = -1;
  switch (event->key())
  {
  case Qt::Key_Tab:
    cameraType = (CameraType) ((cameraType + 1) % 3);
    break;
  case Qt::Key_Equal:
    speed = qMin(speed + 4, 40);
    break;
  case Qt::Key_Minus:
    speed = qMax(speed - 4, 0);
    break;
  case Qt::Key_QuoteLeft:
    builtInOperation = 0;
    break;
  case Qt::Key_1:
    builtInOperation = 1;
    break;
  case Qt::Key_2:
    builtInOperation = 2;
    break;
  case Qt::Key_3:
    builtInOperation = 3;
    break;
  case Qt::Key_4:
    builtInOperation = 4;
    break;
  case Qt::Key_5:
    builtInOperation = 5;
    break;
  case Qt::Key_6:
    builtInOperation = 6;
    break;
  case Qt::Key_7:
    builtInOperation = 7;
    break;
  case Qt::Key_8:
    builtInOperation = 8;
    break;
  case Qt::Key_9:
    builtInOperation = 9;
    break;
  case Qt::Key_0:
    builtInOperation = 10;
    break;
  case Qt::Key_U:
    builtInOperation = 11;
    break;
  case Qt::Key_I:
    builtInOperation = 12;
    break;
  case Qt::Key_O:
    builtInOperation = 13;
    break;
  case Qt::Key_P:
    builtInOperation = 14;
    break;
  case Qt::Key_BracketLeft:
    mirror1 = !mirror1;
    break;
  case Qt::Key_BracketRight:
    mirror0 = !mirror0;
    break;
  case Qt::Key_Backslash:
    mirrorFloor = !mirrorFloor;
    break;
  case Qt::Key_F2:
    if (isFullScreen())
    {
      showNormal();
      setCursor(QCursor(Qt::ArrowCursor));
    }
    else
    {
      showFullScreen();
      setCursor(QCursor(Qt::BlankCursor));
    }
    mouseMoving = false;
    break;
  case Qt::Key_Escape:
    if (isFullScreen())
    {
      showNormal();
      setCursor(QCursor(Qt::ArrowCursor));
      mouseMoving = false;
    }
    break;
  default:
    break;
  }
  if (builtInOperation >= 0)
  {
    mayBeOpenShot = true;
    basketBall->translate = builtInPositions[builtInOperation];
    basketBall->speed = builtInSpeeds[builtInOperation];
  }
  pressed.insert(event->key());
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
  switch (cameraType)
  {
  case TypePlayer:

    break;
  }
  pressed.remove(event->key());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
  if ((!isFullScreen()) && (event->buttons() == Qt::NoButton))
    return;
  if (mouseMoving)
  {
    switch (cameraType)
    {
    case TypePlayer:
      {
        PlayerCamera *camera = (PlayerCamera *) (cameras[(int) cameraType]);
        camera->lookRight((event->pos().x() - lastPosition.x()) * 0.015);
        camera->lookDown((event->pos().y() - lastPosition.y()) * 0.015);
      }
      break;
    }
  }
  else
    mouseMoving = true;
  if (isFullScreen())
  {
    QCursor::setPos(width() / 2, height() / 2);
    lastPosition = QPoint(width() / 2, height() / 2);
  }
  else
    lastPosition = event->pos();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
  lastPosition = event->pos();
  forceAngle = 0;
  if (event->button() == Qt::LeftButton)
    mousePressed = true;
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    mousePressed = false;
  switch (cameraType)
  {
  case TypePlayer:
    if (event->button() == Qt::LeftButton)
    {
      basketBall->translate = cameras[(int) cameraType]->fromPos();
      basketBall->speed = ((cameras[(int) cameraType]->toPos() -
                            basketBall->translate).normalize() +
                           Point3D(0, 0, 0.4)).normalize() *
                          throwSpeed();
      mayBeOpenShot = true;
    }
    break;
  default:
    break;
  }
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
  switch (cameraType)
  {
  case TypeAudience:
    {
      AudienceCamera *camera = (AudienceCamera *) (cameras[(int) cameraType]);
      if (event->delta() < 0)
        camera->zoomOut();
      else
        camera->zoomIn();
    }
    break;
  case TypePlayer:
    break;
  case TypeZ:
    {
      ZCamera *camera = (ZCamera *) (cameras[(int) cameraType]);
      if (event->delta() < 0)
        camera->zoomOut();
      else
        camera->zoomIn();
    }
    break;
  }
}

double GLWidget::throwSpeed()
{
  return qAbs(qSin(forceAngle)) * 20;
}

void GLWidget::paintPower()
{
  if (cameraType != TypePlayer)
    return;

  double percentage = qAbs(qSin(forceAngle));
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);

  glBegin(GL_LINES);
    glColor3ub(50, 220, 50);
    glVertex3f(0, -0.03, -0.5);
    glVertex3f(0, 0.03, -0.5);
    glVertex3f(-0.03, 0, -0.5);
    glVertex3f(0.03, 0, -0.5);
  glEnd();

  glBegin(GL_TRIANGLES);
    glColor3ub(200, 200, 200);
    glVertex3f(-0.25, -0.2, -0.5);
    glVertex3f(-0.25, 0, -0.5);
    glVertex3f(-0.2, 0, -0.5);
  glEnd();

  glBegin(GL_TRIANGLES);
    glColor3ub(0, 255, 0);
    glVertex3f(-0.25, -0.2, -0.5);
    glColor3ub(255 * percentage, 255 - 255 * percentage, 0);
    glVertex3f(-0.25, -0.2 + 0.2 * percentage, -0.5);
    glVertex3f(-0.25 + 0.05 * percentage, -0.2 + 0.2 * percentage, -0.5);
  glEnd();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
}

static float shadowAmbient[4] = {1.0, 1.0, 1.0, 1.0};
static float shadowDiffuse[4] = {1.0, 1.0, 1.0, 1.0};
static float shadowSpecular[4] = {0.0, 0.0, 0.0, 0.0};

void GLWidget::paintShadow()
{
  glPushMatrix();
  glEnable(GL_BLEND);
  glMaterialfv(GL_FRONT, GL_AMBIENT, shadowAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, shadowDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, shadowSpecular);
  Point3D lightPos = Point3D(position[0], position[1], position[2]);
  Point3D lightDir = basketBall->translate - lightPos;
  Point3D shadowCenter = lightPos -
                         (lightDir) *
                         (lightPos._z / (lightDir._z));
  glTranslatef(shadowCenter._x, shadowCenter._y, 0);
  double angle = calculateAngle(lightDir._x, lightDir._y) * 180 / PI + 90;
  glRotated(angle, 0.0, 0.0, 1.0);
  double dxy = qSqrt(lightDir._x * lightDir._x + lightDir._y * lightDir._y);
  angle = qAsin(dxy / lightDir.radius());
  double angle2 = qAtan(basketBall->r / lightDir.radius());
  double length = qAbs((qTan(angle + angle2) - qTan(angle - angle2)) * lightPos._z);
  double scale = length == length ? length / basketBall->r / 2: 1;
  glScaled(1.0, scale, 1.0);
  scale =  basketBall->r * lightPos._z / (lightPos._z - basketBall->translate._z);
  scale = scale == scale ? scale / basketBall->r : 1.0;
  glScaled(scale, 1.0, 1.0);
  double r = basketBall->r;
  glNormal3f(0, 0, 1);
  float centerAlpha = qBound(0.0, 0.85 - basketBall->translate._z / lightPos._z * 1, 1.0);
  glBegin(GL_TRIANGLES);
    for(double angle = 0;angle <= (2.0 * PI);)
    {
      glColor4d(0.0f, 0.0f, 0.0f, 0.0f);
      glVertex3d(r * qSin(angle),
                 r * qCos(angle),
                 0.01);
      angle += 0.1f;
      glVertex3d(r * qSin(angle),
                 r * qCos(angle),
                 0.01);
      glColor4f(0.0f, 0.0f, 0.0f, centerAlpha);
      glVertex3d(0.0, 0.0, 0.01);
    }
  glEnd();
  glPopMatrix();
}

GLWidget::~GLWidget()
{
  timer->stop();
  delete timer;
  delete basket[0];
  delete basket[1];
  delete audience[0];
  delete audience[1];
  delete audience[2];
  delete audience[3];
  delete floor;
  delete walls;
  delete cameras[0];
  delete cameras[1];
  delete cameras[2];
  delete basketBall;
  delete world;
  delete stadium;
}
