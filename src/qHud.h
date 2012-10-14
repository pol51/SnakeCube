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
    QHud(QObject *parent = NULL);
    ~QHud();

    const QString&  text()          const { return _text; }
          int       textAlignment() const { return _textAlignment; }
    const QFont&    font()          const { return _font; }
    const QColor&   fgColor()       const { return _fgColor; }
    const QColor&   bgColor()       const { return _bgColor; }
    const QPoint&   position()      const { return _position; }
    const QSize&    size()          const { return _size; }
          bool      isAutoSize()    const { return _autoSize; }
          int       border()        const { return _border; }
          bool      isVisible()     const { return _visible; }

    void setText(const QString &value)    { _text = value,          _updatable = true; }
    void setTextAlignment(int value)      { _textAlignment = value, _updatable = true; }
    void setFont(const QFont &value)      { _font = value,          _updatable = true; }
    void setFgColor(const QColor &value)  { _fgColor = value,       _updatable = true; }
    void setBgColor(const QColor &value)  { _bgColor = value,       _updatable = true; }
    void setPosition(const QPoint &value) { _position = value; }
    void setSize(const QSize &value)      { _size = value,          _updatable = true; }
    void setAutoSize(const bool value)    { _autoSize = value,      _updatable = true; }
    void setBorder(const int value)       { _border = value,        _updatable = true; }
    void setVisible(const bool value)     { _visible = value; }

    void updateImage();
    void draw(QPainter* painter);

  protected:
    bool _autoSize;
    bool _visible;
    bool _updatable;
    int _textAlignment;
    int _border;
    QImage* _image;
    QString _text;
    QFont _font;
    QColor _fgColor;
    QColor _bgColor;
    QPoint _position;
    QSize _size;
};

#endif
