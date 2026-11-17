#include "cube.h"

#include <QOpenGLFunctions_2_1>

int Cube::_nextId = 0;

Cube::Cube(qreal x, qreal y, qreal z, QObject *parent)
  :QObject(parent),
  _id(_nextId++),
  _x(x), _y(y), _z(z),
  _tx(0.f), _ty(0.f), _tz(0.f),
  _rx(0.f), _ry(0.f), _rz(0.f),
  _size(.1f),
  _color(0xffffffff)
{
}

void Cube::Draw(QOpenGLFunctions_2_1 *gl) const
{
  gl->glPushMatrix();

  const QColor color(_color);

  //if (highlighted())
    //color = _color.lighter(120);

  qreal hSize = _size / 2.f;

  gl->glTranslatef(_x, _y, _z);
  gl->glTranslatef(_tx, _ty, _tz);
  gl->glRotatef(_rx, 1.f, 0.f, 0.f);
  gl->glRotatef(_ry, 0.f, 1.f, 0.f);
  gl->glRotatef(_rz, 0.f, 0.f, 1.f);

  GLfloat diffuseColor[4] =
    {color.redF(), color.greenF(), color.blueF(), color.alphaF()};
  gl->glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);

  gl->glBegin(GL_QUADS);

  gl->glNormal3f( 0.f,  0.f, -1.f);
  gl->glVertex3f( hSize, -hSize, -hSize);
  gl->glVertex3f( hSize,  hSize, -hSize);
  gl->glVertex3f(-hSize,  hSize, -hSize);
  gl->glVertex3f(-hSize, -hSize, -hSize);

  gl->glNormal3f( 0.f,  0.f,  1.f);
  gl->glVertex3f(-hSize, -hSize,  hSize);
  gl->glVertex3f(-hSize,  hSize,  hSize);
  gl->glVertex3f( hSize,  hSize,  hSize);
  gl->glVertex3f( hSize, -hSize,  hSize);

  gl->glNormal3f( 0.f,  1.f, 0.f);
  gl->glVertex3f(-hSize,  hSize,  hSize);
  gl->glVertex3f(-hSize,  hSize, -hSize);
  gl->glVertex3f( hSize,  hSize, -hSize);
  gl->glVertex3f( hSize,  hSize,  hSize);

  gl->glNormal3f( 0.f, -1.f, 0.f);
  gl->glVertex3f(-hSize, -hSize, -hSize);
  gl->glVertex3f(-hSize, -hSize,  hSize);
  gl->glVertex3f( hSize, -hSize,  hSize);
  gl->glVertex3f( hSize, -hSize, -hSize);

  gl->glNormal3f(-1.f,  0.f,  0.f);
  gl->glVertex3f(-hSize, -hSize,  hSize);
  gl->glVertex3f(-hSize, -hSize, -hSize);
  gl->glVertex3f(-hSize,  hSize, -hSize);
  gl->glVertex3f(-hSize,  hSize,  hSize);

  gl->glNormal3f( 1.f,  0.f,  0.f);
  gl->glVertex3f( hSize,  hSize,  hSize);
  gl->glVertex3f( hSize,  hSize, -hSize);
  gl->glVertex3f( hSize, -hSize, -hSize);
  gl->glVertex3f( hSize, -hSize,  hSize);

  gl->glEnd();

  gl->glPopMatrix();
}
