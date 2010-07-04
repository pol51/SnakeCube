#include "glWidget.h"

#include <QtOpenGL>
#include <QDebug>

#include <cube.h>

GlWidget::GlWidget(QWidget *parent)
  :QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
  _fps(0),
  _rx(0), _ry(0), _rz(0),
  _moveX(0), _moveY(0), _moveZ(0),
  _toAdd(99)
{
  _refreshTimer.setSingleShot(false);
  connect(&_refreshTimer, SIGNAL(timeout()), this, SLOT(update()));
  _refreshTimer.start(20);

  _fpsTimer.setSingleShot(false);
  connect(&_fpsTimer, SIGNAL(timeout()), this, SLOT(updateFPS()));
  _fpsTimer.start(1000);

  _gameTimer.setSingleShot(false);
  connect(&_gameTimer, SIGNAL(timeout()), this, SLOT(processGame()));
  _gameTimer.start(200);

  _items.append(new Cube(-.9f,  .9f, .9f, this));
  _items.last()->setColor(0xff7fff7f);

  _plateau.append(new Cube(0.f, 0.f, 0.f, this));
  _plateau.last()->setColor(QColor(0xaf, 0xaf, 0xff, 0xaf));
  _plateau.last()->setSize(1.8f);


  setAttribute(Qt::WA_NoSystemBackground);
  setMinimumSize(400, 400);
  setMouseTracking(true);
  setFocus();
  grabKeyboard();
}

GlWidget::~GlWidget()
{
  makeCurrent();

  QVectorIterator<Cube*> cube(_plateau);
  while (cube.hasNext())
    delete cube.next();
}

QSize GlWidget::sizeHint() const
{
  return QSize(800, 800);
}

void GlWidget::initializeGL()
{
  qglClearColor(Qt::black);
  glShadeModel(GL_SMOOTH);
}

void GlWidget::paintGL()
{
  _fps++;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  resizeGL(width(), height());

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  qglClearColor(Qt::black);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  static GLfloat lightPosition[4] = { 0.2, 0.3, 6.0, 3.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

  glPushMatrix();

  draw();

  glPopMatrix();

  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHTING);

  glFlush();
}

void GlWidget::draw()
{
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslated(0.0, 0.0, -10.0);
  glRotated(_rx / 16.0, 1.0, 0.0, 0.0);
  glRotated(_ry / 16.0, 0.0, 1.0, 0.0);
  glRotated(_rz / 16.0, 0.0, 0.0, 1.0);

  QVectorIterator<Cube*> item(_items);
  while (item.hasNext())
    item.next()->Draw();

  QVectorIterator<Cube*> cube(_plateau);
  while (cube.hasNext())
    cube.next()->Draw();

  glPopMatrix();
}

void GlWidget::updateFPS()
{
  emit FPS(_fps);
  emit titleFPS(QString("SnakeCube [FPS:%1]").arg(_fps));
  _fps = 0;
}

void GlWidget::processGame()
{
  QVectorIterator<Cube*> item(_items);
  Cube *lastCube, *currentCube, *headCube;
  headCube = lastCube = item.next();

  Cube *NewCube = NULL;
  if (_toAdd && (_moveX || _moveY || _moveZ))
  {
    _toAdd--;
    NewCube = new Cube(_items.last()->x(),  _items.last()->y(), _items.last()->z(), this);
    NewCube->setColor(0xff7fff7f);
  }

  item.toBack();
  lastCube = item.previous();
  while (item.hasPrevious())
  {
    currentCube = item.previous();
    lastCube->CopyPosFrom(*currentCube);
    lastCube = currentCube;
  }

  qreal X, Y, Z;
move:
  X = headCube->x();
  Y = headCube->y();
  Z = headCube->z();

  /*if (X > 0.89f) _ry = -(90<<4);
  if (Z > 0.89f) _ry = -(180<<4);
  if (Y > 0.89f) _rx = -(90<<4);*/

  if (_moveX)
  {
    X = headCube->x() + _moveX * 0.1f;
    if (X >  0.91f) { _moveX = 0; _moveZ = (Z>0)?-1:1; goto move; }
    if (X < -0.91f) { _moveX = 0; _moveZ = (Z>0)?-1:1; goto move; }
  }

  if (_moveY)
  {
    Y = headCube->y() + _moveY * 0.1f;
    if (Y >  0.91f) { _moveY = 0; _moveZ = (Z>0)?-1:1; goto move; }
    if (Y < -0.91f) { _moveY = 0; _moveZ = (Z>0)?-1:1; goto move; }
  }

  if (_moveZ)
  {
    Z = headCube->z() + _moveZ * 0.1f;
    if (Z >  0.91f) { _moveZ = 0; _moveX = (X>0)?-1:1; goto move; }
    if (Z < -0.91f) { _moveZ = 0; _moveX = (X>0)?-1:1; goto move; }
  }

  headCube->setX(X);
  headCube->setY(Y);
  headCube->setZ(Z);

  if (NewCube) _items.append(NewCube);
}

void GlWidget::resizeGL(int width, int height)
{
  //int side = qMin(width, height);
  //glViewport((width - side)>>1, (height - side)>>1, side, side);
  glViewport(0, 0, width, height);

  float x = 1.5;
  float y = 1.5;

  if (width > height)
    x *= ((float)width / (float)height);
  else
    y *= ((float)height / (float)width);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-x, +x, +y, -y, -2.0, 15.0);
  glMatrixMode(GL_MODELVIEW);
}

void GlWidget::paintEvent(QPaintEvent */*event*/)
{
  QPainter painter;
  painter.begin(this);

  paintGL();

  painter.end();
}

void GlWidget::mouseMoveEvent(QMouseEvent *event)
{
  int dx = event->x() - _lastPos.x();
  int dy = event->y() - _lastPos.y();

  if (event->buttons() & Qt::LeftButton)
  {
    _rx = normalizeAngle(_rx + 8*dy);
    _ry = normalizeAngle(_ry + 8*dx);
  }
  else if (event->buttons() & Qt::RightButton)
  {
    _rx = normalizeAngle(_rx + 8*dy);
    _rz = normalizeAngle(_rz + 8*dx);
  }
  _lastPos = event->pos();
}

void GlWidget::mousePressEvent(QMouseEvent *event)
{
  _lastPos = event->pos();
}

void GlWidget::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Left)  { _moveX = -1; _moveY =  0; _moveZ =  0; }
  if (event->key() == Qt::Key_Right) { _moveX =  1; _moveY =  0; _moveZ =  0; }
  if (event->key() == Qt::Key_Up)    { _moveX =  0; _moveY =  1; _moveZ =  0; }
  if (event->key() == Qt::Key_Down)  { _moveX =  0; _moveY = -1; _moveZ =  0; }
}

int GlWidget::normalizeAngle(int angle)
{
  while (angle < 0) angle += (360<<4);
  while (angle > (360<<4)) angle -= (360<<4);
  return angle;
}
