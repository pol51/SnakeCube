#include "glWidget.h"

#include <QtOpenGL>

#include <cube.h>
#include <qHud.h>

QString GlWidget::_axeName[6] = { "X-", "X+", "Y-", "Y+", "Z-", "Z+" };

GlWidget::GlWidget(QWidget *parent)
  :QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer), parent),
  _fps(0),
  _rx(-10<<4), _ry(10<<4), _rz(0),
  _rxg(0), _ryg(0), _rzg(0),
  _rxgc(0), _rygc(0), _rzgc(0),
  _rxa(0), _rya(0),
  _moveA(0), _moveB(0),
  _moveX(0), _moveY(0), _moveZ(0),
  _toAdd(9),
  _axeA(eXp), _axeB(eYn)
{
  _refreshTimer.setSingleShot(false);
  connect(&_refreshTimer, SIGNAL(timeout()), this, SLOT(update()));
  _refreshTimer.start(20);

  _fpsTimer.setSingleShot(false);
  connect(&_fpsTimer, SIGNAL(timeout()), this, SLOT(updateFPS()));
  _fpsTimer.start(1000);

  _gameTimer.setSingleShot(false);
  connect(&_gameTimer, SIGNAL(timeout()), this, SLOT(processGame()));
  _gameTimer.start(100);

  _cameraTimer.setSingleShot(false);
  connect(&_cameraTimer, SIGNAL(timeout()), this, SLOT(updateCamera()));
  _cameraTimer.start(20);


  _items.append(new Cube(0.f, 0.f, 0.9f, this));
  _items.last()->setColor(0xff7fff7f);

  _plateau = new Cube(0.f, 0.f, 0.f, this);
  _plateau->setColor(QColor(0xaf, 0xaf, 0xff, 0xaf));
  _plateau->setSize(1.8f);


  setAttribute(Qt::WA_NoSystemBackground);
  setMinimumSize(160, 160);
  setMouseTracking(true);
  setFocus();
  grabKeyboard();

  _gameHud = new QHud(this);
  QFont font;
  font.setFamily("DejaVu Sans Mono");
  font.setPixelSize(12);
  font.setBold(true);
  _gameHud->setFont(font);
  _gameHud->setForeColor(QColor::fromRgb(0, 255, 127));
  _gameHud->setPosition(QPoint(5, 5));
  _gameHud->setVisible(true);
}

GlWidget::~GlWidget()
{
  makeCurrent();

  delete _plateau;
  delete _gameHud;

  QVectorIterator<Cube*> item(_items);
  while (item.hasNext())
    delete item.next();

}

QSize GlWidget::sizeHint() const
{
  return QSize(800, 800);
}

void GlWidget::initializeGL()
{
  glShadeModel(GL_SMOOTH);
  static GLfloat lightPosition[4] = { 0.2, 0.3, 6.0, 3.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GlWidget::paintGL()
{
  ++_fps;

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glPushMatrix();

  draw();

  glPopMatrix();

  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHTING);
  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glFlush();
}

void GlWidget::draw()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glTranslated(0, 0, -10);

  glRotated(_rx>>4, 1, 0, 0);
  glRotated(_ry>>4, 0, 1, 0);
  glRotated(_rz>>4, 0, 0, 1);

  glRotated(_rxa>>4, 1, 0, 0);
  glRotated(_rya>>4, 0, 1, 0);

  glRotated(_rxg>>4, 1, 0, 0);
  glRotated(_ryg>>4, 0, 1, 0);
  glRotated(_rzg>>4, 0, 0, 1);

  QVectorIterator<Cube*> item(_items);
  while (item.hasNext())
    item.next()->Draw();

  _plateau->Draw();
}

void GlWidget::updateFPS()
{
  setWindowTitle(QString("SnakeCube [FPS:%1]").arg(_fps));
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

  if (_moveX)
  {
    X = headCube->x() + _moveX * 0.1f;
    if (X >  0.91f) { rotateCube(eXp); convertMove(); goto move; }
    if (X < -0.91f) { rotateCube(eXn); convertMove(); goto move; }
  }

  if (_moveY)
  {
    Y = headCube->y() + _moveY * 0.1f;
    if (Y >  0.91f) { rotateCube(eYp); convertMove(); goto move; }
    if (Y < -0.91f) { rotateCube(eYn); convertMove(); goto move; }
  }

  if (_moveZ)
  {
    Z = headCube->z() + _moveZ * 0.1f;
    if (Z >  0.91f) { rotateCube(eZp); convertMove(); goto move; }
    if (Z < -0.91f) { rotateCube(eZn); convertMove(); goto move; }
  }

  headCube->setX(X);
  headCube->setY(Y);
  headCube->setZ(Z);

  if (NewCube) _items.append(NewCube);
}

void GlWidget::rotateCube(Axe endAxe)
{
  // animation
  if (int(endAxe)>>1 == int(_axeA)>>1)
    _rya = (endAxe == _axeA) ? 90<<4 : -90<<4;
  if (int(endAxe)>>1 == int(_axeB)>>1)
    _rxa = (endAxe == _axeB) ? 90<<4 : -90<<4;

  // change axes
  switch (endAxe)
  {
    case eXn:
    case eXp:
      if (_axeA == eXn || _axeA == eXp)
        if (_axeB == eYn || _axeB == eYp)
          _axeA = ((_items.first()->z()>0)==(_moveA>0))?eZn:eZp;
        else
          _axeA = ((_items.first()->y()>0)==(_moveA>0))?eYn:eYp;
      else
        if (_axeA == eYn || _axeA == eYp)
          _axeB = ((_items.first()->z()>0)==(_moveB>0))?eZn:eZp;
        else
          _axeB = ((_items.first()->y()>0)==(_moveB>0))?eYn:eYp;
      break;

    case eYn:
    case eYp:
      if (_axeA == eYn || _axeA == eYp)
        if (_axeB == eXn || _axeB == eXp)
          _axeA = ((_items.first()->z()>0)==(_moveA>0))?eZn:eZp;
        else
          _axeA = ((_items.first()->x()>0)==(_moveA>0))?eXn:eXp;
      else
        if (_axeA == eXn || _axeA == eXp)
          _axeB = ((_items.first()->z()>0)==(_moveB>0))?eZn:eZp;
        else
          _axeB = ((_items.first()->x()>0)==(_moveB>0))?eXn:eXp;
      break;

    case eZn:
    case eZp:
      if (_axeA == eZn || _axeA == eZp)
        if (_axeB == eYn || _axeB == eYp)
          _axeA = ((_items.first()->x()>0)==(_moveA>0))?eXn:eXp;
        else
          _axeA = ((_items.first()->y()>0)==(_moveA>0))?eYn:eYp;
      else
        if (_axeA == eYn || _axeA == eYp)
          _axeB = ((_items.first()->x()>0)==(_moveB>0))?eXn:eXp;
        else
          _axeB = ((_items.first()->y()>0)==(_moveB>0))?eYn:eYp;
      break;
  }

  // move game camera
  if (_axeA == eXp && _axeB == eYp) _rxg = 180<<4,  _ryg = 0,       _rzg = 0;
  if (_axeA == eXp && _axeB == eYn) _rxg = 0,       _ryg = 0,       _rzg = 0;
  if (_axeA == eXp && _axeB == eZp) _rxg = 90<<4,   _ryg = 0,       _rzg = 0;
  if (_axeA == eXp && _axeB == eZn) _rxg = 270<<4,  _ryg = 0,       _rzg = 0;

  if (_axeA == eYp && _axeB == eXp) _rxg = 0,       _ryg = 0,       _rzg = 270<<4;
  if (_axeA == eYp && _axeB == eXn) _rxg = 0,       _ryg = 180<<4,  _rzg = 90<<4;
  if (_axeA == eYp && _axeB == eZp) _rxg = 270<<4,  _ryg = 180<<4,  _rzg = 90<<4;
  if (_axeA == eYp && _axeB == eZn) _rxg = 270<<4,  _ryg = 0,       _rzg = 270<<4;

  if (_axeA == eZp && _axeB == eXp) _rxg = 270<<4,  _ryg = 90<<4,   _rzg = 0;
  if (_axeA == eZp && _axeB == eXn) _rxg = 90<<4,   _ryg = 90<<4,   _rzg = 0;
  if (_axeA == eZp && _axeB == eYp) _rxg = 180<<4,  _ryg = 90<<4,   _rzg = 0;
  if (_axeA == eZp && _axeB == eYn) _rxg = 0,       _ryg = 90<<4,   _rzg = 0;

  if (_axeA == eXn && _axeB == eYp) _rxg = 180<<4,  _ryg = 180<<4,  _rzg = 0;
  if (_axeA == eXn && _axeB == eYn) _rxg = 0,       _ryg = 180<<4,  _rzg = 0;
  if (_axeA == eXn && _axeB == eZp) _rxg = 270<<4,  _ryg = 180<<4,  _rzg = 0;
  if (_axeA == eXn && _axeB == eZn) _rxg = 90<<4,   _ryg = 180<<4,  _rzg = 0;

  if (_axeA == eYn && _axeB == eXp) _rxg = 180<<4,  _ryg = 0,       _rzg = 90<<4;
  if (_axeA == eYn && _axeB == eXn) _rxg = 0,       _ryg = 0,       _rzg = 90<<4;
  if (_axeA == eYn && _axeB == eZp) _rxg = 90<<4,   _ryg = 0,       _rzg = 90<<4;
  if (_axeA == eYn && _axeB == eZn) _rxg = 270<<4,  _ryg = 0,       _rzg = 90<<4;

  if (_axeA == eZn && _axeB == eXp) _rxg = 90<<4,   _ryg = 270<<4,  _rzg = 0;
  if (_axeA == eZn && _axeB == eXn) _rxg = 270<<4,  _ryg = 270<<4,  _rzg = 0;
  if (_axeA == eZn && _axeB == eYp) _rxg = 180<<4,  _ryg = 270<<4,  _rzg = 0;
  if (_axeA == eZn && _axeB == eYn) _rxg = 0,       _ryg = 270<<4,  _rzg = 0;

  // update hub
  _gameHud->setText(QString("A: %1\nB: %2\n\nX: %3\nY: %4\nZ: %5").arg(_axeName[_axeA]).arg(_axeName[_axeB]).arg(_rxg>>4).arg(_ryg>>4).arg(_rzg>>4));
}

void GlWidget::resizeGL(int width, int height)
{
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
  painter.setRenderHint(QPainter::Antialiasing);

  paintGL();

  _gameHud->draw(&painter);

  painter.end();
}

void GlWidget::mouseMoveEvent(QMouseEvent *event)
{
  int dx = event->x() - _lastPos.x();
  int dy = event->y() - _lastPos.y();

  if (event->buttons() & Qt::LeftButton)
  {
    _rx = normalizeAngle(_rx + (dy<<3));
    _ry = normalizeAngle(_ry + (dx<<3));
  }
  else if (event->buttons() & Qt::RightButton)
  {
    _rx = normalizeAngle(_rx + (dy<<3));
    _rz = normalizeAngle(_rz + (dx<<3));
  }
  _lastPos = event->pos();
}

void GlWidget::mousePressEvent(QMouseEvent *event)
{
  _lastPos = event->pos();
}

void GlWidget::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Left)  { _moveA = -1; _moveB =  0; }
  if (event->key() == Qt::Key_Right) { _moveA =  1; _moveB =  0; }
  if (event->key() == Qt::Key_Up)    { _moveA =  0; _moveB =  1; }
  if (event->key() == Qt::Key_Down)  { _moveA =  0; _moveB = -1; }

  if (event->key() == Qt::Key_Escape){ close(); }

  if (event->key() == Qt::Key_F) { setWindowState(windowState() ^ Qt::WindowFullScreen);}

  if (event->key() == Qt::Key_Space)
  {

    if (_gameTimer.isActive())
      _gameTimer.stop();
    else
      _gameTimer.start();
  }

  convertMove();
}

void GlWidget::convertMove()
{
  _moveX = _moveY = _moveZ =  0;

  switch (_axeA)
  {
    case eXn: _moveX = -_moveA; break;
    case eXp: _moveX =  _moveA; break;
    case eYn: _moveY = -_moveA; break;
    case eYp: _moveY =  _moveA; break;
    case eZn: _moveZ = -_moveA; break;
    case eZp: _moveZ =  _moveA; break;
  }

  switch (_axeB)
  {
    case eXn: _moveX = -_moveB; break;
    case eXp: _moveX =  _moveB; break;
    case eYn: _moveY = -_moveB; break;
    case eYp: _moveY =  _moveB; break;
    case eZn: _moveZ = -_moveB; break;
    case eZp: _moveZ =  _moveB; break;
  }
}

void GlWidget::updateCamera()
{
  /*int Drx = _rxg - _rxgc; while (Drx > 90<<4) Drx-=360<<4; while (Drx < -90<<4) Drx+=360<<4;
  if (Drx) { if (Drx > 0) _rxgc += (9<<4);  else _rxgc -= (9<<4); if (_rxgc == _rxg) _rxgc = _rxg = normalizeAngle(_rxg); }
  int Dry = _ryg - _rygc; while (Dry > 90<<4) Dry-=360<<4; while (Dry < -90<<4) Dry+=360<<4;
  if (Dry) { if (Dry > 0) _rygc += (9<<4);  else _rygc -= (9<<4); if (_rygc == _ryg) _rygc = _ryg = normalizeAngle(_ryg); }
  int Drz = _rzg - _rzgc; while (Drz > 90<<4) Drz-=360<<4; while (Drz < -90<<4) Drz+=360<<4;
  if (Drz) { if (Drz > 0) _rzgc += (9<<4);  else _rzgc -= (9<<4); if (_rzgc == _rzg) _rzgc = _rzg = normalizeAngle(_rzg); }*/
  if (_rxa > 0) _rxa -= (9<<4);
  if (_rxa < 0) _rxa += (9<<4);
  if (_rya > 0) _rya -= (9<<4);
  if (_rya < 0) _rya += (9<<4);
}

int GlWidget::normalizeAngle(int angle)
{
  while (angle < 0)         angle += (360<<4);
  while (angle >= (360<<4))  angle -= (360<<4);
  return angle;
}
