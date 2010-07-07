#include <QApplication>

#include "glWidget.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  GlWidget myGlw;

  myGlw.show();

  return app.exec();
}
