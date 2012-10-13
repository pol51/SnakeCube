#include <QtGui/QPainter>
#include <QtGui/QImage>

#include "qHud.h"

QHud::QHud(QObject *parent)
  : QObject(parent)
{
  _image = NULL;
  _textAlignment = Qt::AlignLeft;
  _foreColor = Qt::white;
  _backColor = QColor::fromRgb(0, 0, 0, 95);	//semi-transparent
  _position.setX(0);
  _position.setY(0);
  _size.setHeight(0);
  _size.setWidth(0);
  _autoSize = true;
  _border = 4;
  _visible = false;
  _updatable = true;
}

QHud::~QHud()
{
  if (_image)
    delete _image;
}

const QString & QHud::getText() const
{
  return _text;
}

void QHud::setText(QString value)
{
  _text = value;
  _updatable = true;
}

const int QHud::getTextAlignment() const
{
  return _textAlignment;
}

void QHud::setTextAlignment(int value)
{
  _textAlignment = value;
  _updatable = true;
}

const QFont & QHud::getFont() const
{
  return _font;
}

void QHud::setFont(QFont value)
{
  _font = value;
  _updatable = true;
}

const QColor & QHud::getForeColor() const
{
  return _foreColor;
}

void QHud::setForeColor(QColor value)
{
  _foreColor = value;
  _updatable = true;
}

const QColor & QHud::getBackColor() const
{
  return _backColor;
}

void QHud::setBackColor(QColor value)
{
  _backColor = value;
  _updatable = true;
}

const QPoint & QHud::getPosition() const
{
  return _position;
}

void QHud::setPosition(QPoint value)
{
  _position = value;
}

const QSize & QHud::getSize() const
{
  return _size;
}

void QHud::setSize(QSize value)
{
  _size = value;
  _updatable = true;
}

const bool QHud::isAutoSize() const
{
  return _autoSize;
}

void QHud::setAutoSize(bool value)
{
  _autoSize = value;
  _updatable = true;
}

const int QHud::getBorder() const
{
  return _border;
}

void QHud::setBorder(int value)
{
  _border = value;
  _updatable = true;
}

const bool QHud::isVisible() const
{
  return _visible;
}

void QHud::setVisible(bool value)
{
  _visible = value;
}

void QHud::updateImage()
{
  if (!_updatable)
    return;

  QSize size;

  if (_image)
    size = _image->size();

  if (_autoSize)
  {
    QFontMetrics metrics = QFontMetrics(_font);
    QRect rect = metrics.boundingRect(0, 0, 5000, 5000,
              _textAlignment, _text);
    _size = rect.size() + 2 * QSize(_border, _border);
  }

  if (size != _size)
  {
    delete _image;
    _image = new QImage(_size, QImage::Format_ARGB32_Premultiplied);
  }

  if (_image)
  {
    _image->fill(_backColor.rgba());

    QPainter painter;
    painter.begin(_image);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setPen(_foreColor.rgba());
    painter.setFont(_font);
    painter.drawText(_border, _border, _image->width() - _border,
                _image->height() - _border, _textAlignment, _text);
    painter.end();
  }

  _updatable = false;
}

void QHud::draw(QPainter* painter)
{
  if (painter && true)
  {
    updateImage();
    painter->drawImage(_position, *_image);
  }
}
