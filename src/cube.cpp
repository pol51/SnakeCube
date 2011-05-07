#include "cube.h"

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

void Cube::Draw() const
{
  glPushMatrix();

  glLoadName(_id);

  QColor color(_color);

  //if (highlighted())
    //color = _color.lighter(120);

  qreal hSize = _size / 2.f;

  glTranslatef(_x, _y, _z);
  glTranslatef(_tx, _ty, _tz);
  glRotatef(_rx, 1.f, 0.f, 0.f);
  glRotatef(_ry, 0.f, 1.f, 0.f);
  glRotatef(_rz, 0.f, 0.f, 1.f);

  GLfloat diffuseColor[4] =
    {color.redF(), color.greenF(), color.blueF(), color.alphaF()};
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);

  glBegin(GL_QUADS);

  glNormal3f( 0.f,  0.f, -1.f);
  glVertex3f( hSize, -hSize, -hSize);
  glVertex3f( hSize,  hSize, -hSize);
  glVertex3f(-hSize,  hSize, -hSize);
  glVertex3f(-hSize, -hSize, -hSize);

  glNormal3f( 0.f,  0.f,  1.f);
  glVertex3f(-hSize, -hSize,  hSize);
  glVertex3f(-hSize,  hSize,  hSize);
  glVertex3f( hSize,  hSize,  hSize);
  glVertex3f( hSize, -hSize,  hSize);

  glNormal3f( 0.f,  1.f, 0.f);
  glVertex3f(-hSize,  hSize,  hSize);
  glVertex3f(-hSize,  hSize, -hSize);
  glVertex3f( hSize,  hSize, -hSize);
  glVertex3f( hSize,  hSize,  hSize);

  glNormal3f( 0.f, -1.f, 0.f);
  glVertex3f(-hSize, -hSize, -hSize);
  glVertex3f(-hSize, -hSize,  hSize);
  glVertex3f( hSize, -hSize,  hSize);
  glVertex3f( hSize, -hSize, -hSize);

  glNormal3f(-1.f,  0.f,  0.f);
  glVertex3f(-hSize, -hSize,  hSize);
  glVertex3f(-hSize, -hSize, -hSize);
  glVertex3f(-hSize,  hSize, -hSize);
  glVertex3f(-hSize,  hSize,  hSize);

  glNormal3f( 1.f,  0.f,  0.f);
  glVertex3f( hSize,  hSize,  hSize);
  glVertex3f( hSize,  hSize, -hSize);
  glVertex3f( hSize, -hSize, -hSize);
  glVertex3f( hSize, -hSize,  hSize);

  glEnd();

  glPopMatrix();
}
