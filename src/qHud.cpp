#include <QtGui/QPainter>
#include <QtGui/QImage>

#include "qHud.h"

QHud::QHud(QObject *parent)
  : QObject(parent),
  _autoSize(true), _visible(false), _updatable(true),
  _textAlignment(Qt::AlignLeft), _border(4),
  _image(NULL),
  _fgColor(Qt::white),
  _bgColor(QColor::fromRgb(0, 0, 0, 95)),
  _position(QPoint(0, 0)),
  _size(QSize(0, 0))
{
}

QHud::~QHud()
{
 if (_image)
   delete _image;
}

void QHud::updateImage()
{
  if (_autoSize)
  {
    const QFontMetrics Metrics(_font);
    const QRect Rect(Metrics.boundingRect(0, 0, 5000, 5000, _textAlignment, _text));
    _size = Rect.size() + 2 * QSize(_border, _border);
  }

  if (!_image || _image->size() != _size)
  {
    if (_image) delete _image;
    _image = new QImage(_size, QImage::Format_ARGB32_Premultiplied);
  }

  _image->fill(_bgColor.rgba());

  static QPainter painter;
  painter.begin(_image);
  painter.setRenderHint(QPainter::TextAntialiasing);
  painter.setPen(_fgColor.rgba());
  painter.setFont(_font);
  painter.drawText(_border, _border,
                   _image->width()  - _border,
                   _image->height() - _border,
                   _textAlignment,
                   _text);
  painter.end();

  _updatable = false;
}

void QHud::draw(QPainter* painter)
{
  if (_updatable) updateImage();
  painter->drawImage(_position, *_image);
}
