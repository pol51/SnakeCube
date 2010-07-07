#include <QApplication>

#include <QWidget>
#include <QGridLayout>
#include <QSlider>
#include <QCheckBox>

#include "glWidget.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QWidget mainWindow;

  GlWidget myGlw;

  mainWindow.connect(&myGlw, SIGNAL(titleFPS(QString)), &mainWindow, SLOT(setWindowTitle(const QString)));
  mainWindow.connect(&myGlw, SIGNAL(askExit()), &mainWindow, SLOT(close()));

  QGridLayout mainLayout;
  mainLayout.addWidget(&myGlw, 0, 0, 1, 1);
  mainWindow.setLayout(&mainLayout);
  mainWindow.setWindowTitle("SnakeCube");
  mainWindow.show();

  return app.exec();
}
