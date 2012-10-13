#ifndef __QTHUD_H__
#define __QTHUD_H__

#include <QtCore/QObject>
#include <QtGui/QFont>
#include <QtGui/QColor>
#include <QtCore/QString>

#include <QtCore/QPoint>
#include <QtCore/QSize>

class QImage;
class QPainter;


class QHud : public QObject
{

Q_OBJECT

public:

  QHud(QObject *parent = 0);
  virtual ~QHud();

  const QString & getText() const;
  const int getTextAlignment() const;
  const QFont & getFont() const;
  const QColor & getForeColor() const;
  const QColor & getBackColor() const;
  const QPoint & getPosition() const;
  const QSize & getSize() const;
  const bool isAutoSize() const;
  const int getBorder() const;
  const bool isVisible() const;

  void setText(QString value);
  void setTextAlignment(int value);
  void setFont(QFont value);
  void setForeColor(QColor value);
  void setBackColor(QColor value);
  void setPosition(QPoint value);
  void setSize(QSize value);
  void setAutoSize(bool value);
  void setBorder(int value);
  void setVisible(bool value);

  void updateImage();
  void draw(QPainter* painter);

private:

  QString _text;
  QImage* _image;
  int _textAlignment;
  QFont _font;
  QColor _foreColor;
  QColor _backColor;
  QPoint _position;
  QSize _size;
  bool _autoSize;
  int _border;
  bool _visible;
  bool _updatable;
};

#endif
