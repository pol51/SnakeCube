TEMPLATE =		app

TARGET =		SnakeCube

DEPENDPATH +=	src
INCLUDEPATH +=	src

QT +=			opengl

HEADERS =\
  src/glWidget.h\
  src/cube.h

SOURCES =\
  src/glWidget.cpp\
  src/cube.cpp\
  src/main.cpp

CONFIG +=\
  release \
  warn_on \
  qt \
  console \
  gui

DESTDIR =		bin
MOC_DIR =		build/moc
UI_DIR =		build/uic
OBJECTS_DIR =	build/obj
RCC_DIR =		build/rcc
