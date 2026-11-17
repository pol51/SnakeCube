#include <QApplication>
#include <QSurfaceFormat>

#include "glWidget.h"

int main(int argc, char *argv[])
{
  QSurfaceFormat format;
  format.setVersion(2, 1);
  format.setProfile(QSurfaceFormat::CompatibilityProfile);
  format.setSamples(4);
  QSurfaceFormat::setDefaultFormat(format);

  QApplication app(argc, argv);

  GlWidget myGlw;

  myGlw.show();

  return app.exec();
}
