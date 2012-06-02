#include <QtGui/QApplication>
#include "glwidget.h"
#include "soundcontroller.h"

int main(int argc, char *argv[])
{
  QTime time;
  time= QTime::currentTime();
  qsrand(time.msec()+time.second()*1000);
  QApplication a(argc, argv);
  SoundController::init();
  GLWidget w;
  w.setGeometry(100,100,800,500);
  w.show();
  return a.exec();
}
