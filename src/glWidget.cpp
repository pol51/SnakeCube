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
  _toAdd(0), _score(-1),
  _axeA(eXp), _axeB(eYn),
  _debugHud(new QHud(this)),
  _gameHud(new QHud(this))
{
  _refreshTimer.setSingleShot(false);
  connect(&_refreshTimer, SIGNAL(timeout()), this, SLOT(update()));
  _refreshTimer.start(1000/120);

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

  _food = new Cube(0.f, 0.f, 0.9f, this);
  _food->setColor(0xffff7f7f);

  _plateau = new Cube(0.f, 0.f, 0.f, this);
  _plateau->setColor(QColor(0xaf, 0xaf, 0xff, 0xaf));
  _plateau->setSize(1.8f);


  setAttribute(Qt::WA_NoSystemBackground);
  setMinimumSize(160, 160);
  setMouseTracking(true);
  setFocus();
  grabKeyboard();

  _debugHud->setFont(QFont("DejaVu Sans Mono", 12, QFont::Bold));
  _debugHud->setFgColor(QColor::fromRgb(0, 127, 255));
  _debugHud->setText(QString("A: %1\nB: %2\n\nX: %3\nY: %4\nZ: %5").arg(_axeName[_axeA]).arg(_axeName[_axeB]).arg(360).arg(360).arg(360));
  _debugHud->updateImage();
  _debugHud->setPosition(QPoint(width() - _debugHud->size().width() - 5, 5));
  _debugHud->setText(QString("A: %1\nB: %2\n\nX: %3\nY: %4\nZ: %5").arg(_axeName[_axeA]).arg(_axeName[_axeB]).arg(_rxg>>4).arg(_ryg>>4).arg(_rzg>>4));

  _gameHud->setFont(QFont("DejaVu Sans Mono", 12, QFont::Bold));
  _gameHud->setFgColor(QColor::fromRgb(0, 255, 127));
  _gameHud->setPosition(QPoint(5, 5));
  _gameHud->setText(QString("Score: %1").arg(_score));
  _gameHud->setVisible(true);
}

GlWidget::~GlWidget()
{
  makeCurrent();

  delete _plateau;
  delete _debugHud;

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

  _food->Draw();

  _plateau->Draw();
}

void GlWidget::updateFPS()
{
  setWindowTitle(QString("SnakeCube [FPS:%1]").arg(_fps));
  _fps = 0;
}

void GlWidget::eatFood()
{
  QVectorIterator<Cube*> Item(_items);
  qreal X, Y, Z;

  // increase score and snake length
  ++_score;
  _toAdd = 5;

  // update game hud
  _gameHud->setText(QString("Score: %1").arg(_score));

randomize:
  X = (qrand() % 19 - 9) / 10.f;
  Y = (qrand() % 19 - 9) / 10.f;
  Z = (qrand() % 19 - 9) / 10.f;

  switch (qrand() % 6)
  {
    case 0: X = -0.9f; break;
    case 1: X =  0.9f; break;
    case 2: Y = -0.9f; break;
    case 3: Y =  0.9f; break;
    case 4: Z = -0.9f; break;
    case 5: Z =  0.9f; break;
  }

  _food->setY(Y);
  _food->setX(X);
  _food->setZ(Z);

  // check if the food isn't isn the snake
  Item.toBack();
  while (Item.hasPrevious())
    if (*_food == *Item.previous())
      goto randomize;
}

void GlWidget::processGame()
{
  QVectorIterator<Cube*> Item(_items);
  Cube *LastCube, *CurrentCube, *HeadCube;
  HeadCube = LastCube = Item.next();

  // create new tail item if needed
  Cube *NewCube(NULL);
  if (_toAdd && (_moveX || _moveY || _moveZ))
  {
    _toAdd--;
    NewCube = new Cube(_items.last()->x(),  _items.last()->y(), _items.last()->z(), this);
    NewCube->setColor(0xff7fff7f);
  }

  // move body
  Item.toBack();
  LastCube = Item.previous();
  while (Item.hasPrevious())
  {
    CurrentCube = Item.previous();
    LastCube->CopyPosFrom(*CurrentCube);
    LastCube = CurrentCube;
  }

  // move head
  qreal X, Y, Z;
move:
  X = HeadCube->x();
  Y = HeadCube->y();
  Z = HeadCube->z();

  if (_moveX)
  {
    X = HeadCube->x() + _moveX * 0.1f;
    if (X >  0.91f) { rotateCube(eXp); convertMove(); goto move; }
    if (X < -0.91f) { rotateCube(eXn); convertMove(); goto move; }
  }

  if (_moveY)
  {
    Y = HeadCube->y() + _moveY * 0.1f;
    if (Y >  0.91f) { rotateCube(eYp); convertMove(); goto move; }
    if (Y < -0.91f) { rotateCube(eYn); convertMove(); goto move; }
  }

  if (_moveZ)
  {
    Z = HeadCube->z() + _moveZ * 0.1f;
    if (Z >  0.91f) { rotateCube(eZp); convertMove(); goto move; }
    if (Z < -0.91f) { rotateCube(eZn); convertMove(); goto move; }
  }

  HeadCube->setX(X);
  HeadCube->setY(Y);
  HeadCube->setZ(Z);

  // increase snake size (enqueue tail item)
  if (NewCube) _items.append(NewCube);

  // check colisions
  Item.toBack();
  while (Item.hasPrevious())
  {
    CurrentCube = Item.previous();
    if (*HeadCube == *CurrentCube && HeadCube != CurrentCube)
       _gameTimer.stop();
    if (*HeadCube == *_food)
      eatFood();
  }
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
  switch(_axeA)
  {
    case eXp:
      _ryg = 0, _rzg = 0;
      switch(_axeB)
      {
        case eYp: _rxg = 180<<4;  break;
        case eYn: _rxg = 0;       break;
        case eZp: _rxg = 90<<4;   break;
        case eZn: _rxg = 270<<4;  break;
        default:  break;
      }
      break;
    case eYp:
      switch(_axeB)
      {
        case eXp: _rxg = 0,       _ryg = 0,       _rzg = 270<<4;  break;
        case eXn: _rxg = 0,       _ryg = 180<<4,  _rzg = 90<<4;   break;
        case eZp: _rxg = 270<<4,  _ryg = 180<<4,  _rzg = 90<<4;   break;
        case eZn: _rxg = 270<<4,  _ryg = 0,       _rzg = 270<<4;  break;
        default:  break;
      }
      break;
    case eZp:
      _ryg = 90<<4, _rzg = 0;
      switch(_axeB)
      {
        case eXp: _rxg = 270<<4;  break;
        case eXn: _rxg = 90<<4;   break;
        case eYp: _rxg = 180<<4;  break;
        case eYn: _rxg = 0;       break;
        default:  break;
      }
      break;
    case eXn:
      _ryg = 180<<4, _rzg = 0;
      switch(_axeB)
      {
        case eYp: _rxg = 180<<4;  break;
        case eYn: _rxg = 0;       break;
        case eZp: _rxg = 270<<4;  break;
        case eZn: _rxg = 90<<4;   break;
        default:  break;
      }
      break;
    case eYn:
      _ryg = 0, _rzg = 90<<4;
      switch(_axeB)
      {
        case eXp: _rxg = 180<<4;  break;
        case eXn: _rxg = 0;       break;
        case eZp: _rxg = 90<<4;   break;
        case eZn: _rxg = 270<<4;  break;
        default:  break;
      }
      break;
    case eZn:
      _ryg = 270<<4, _rzg = 0;
      switch(_axeB)
      {
        case eXp: _rxg = 90<<4;   break;
        case eXn: _rxg = 270<<4;  break;
        case eYp: _rxg = 180<<4;  break;
        case eYn: _rxg = 0;       break;
        default:  break;
      }
      break;
  }

  // update debug hub
  _debugHud->setText(QString("A: %1\nB: %2\n\nX: %3\nY: %4\nZ: %5").arg(_axeName[_axeA]).arg(_axeName[_axeB]).arg(_rxg>>4).arg(_ryg>>4).arg(_rzg>>4));
}

void GlWidget::resizeGL(int width, int height)
{
  glViewport(0, 0, width, height);

  float X = 1.5;
  float Y = 1.5;

  if (width > height)
    X *= ((float)width / (float)height);
  else
    Y *= ((float)height / (float)width);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-X, +X, +Y, -Y, -2.0, 15.0);

  _debugHud->setPosition(QPoint(width - _debugHud->size().width() - 5, 5));
}

void GlWidget::paintEvent(QPaintEvent */*event*/)
{
  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);

  paintGL();

  _debugHud->draw(&painter);
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
  switch (event->key())
  {
    case Qt::Key_Left:    _moveA = -1, _moveB =  0; convertMove(); break;
    case Qt::Key_Right:   _moveA =  1, _moveB =  0; convertMove(); break;
    case Qt::Key_Up:      _moveA =  0, _moveB =  1; convertMove(); break;
    case Qt::Key_Down:    _moveA =  0, _moveB = -1; convertMove(); break;

    case Qt::Key_Escape:  close(); break;

    case Qt::Key_D:       _debugHud->setVisible(!_debugHud->isVisible());  break;

    case Qt::Key_F:       setWindowState(windowState() ^ Qt::WindowFullScreen); break;

    case Qt::Key_Space:   if (_gameTimer.isActive())  _gameTimer.stop();
                          else                        _gameTimer.start(); break;
    default:  break;
  }
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
  if (_rxa > 0) _rxa -= (9<<4);
  if (_rxa < 0) _rxa += (9<<4);
  if (_rya > 0) _rya -= (9<<4);
  if (_rya < 0) _rya += (9<<4);
}

int GlWidget::normalizeAngle(int angle)
{
  while (angle < 0)         angle += (360<<4);
  while (angle >= (360<<4)) angle -= (360<<4);
  return angle;
}
