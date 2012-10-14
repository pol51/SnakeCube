#ifndef __CUBE_H
#define __CUBE_H

#include <QtOpenGL>

class Cube : public QObject
{
  Q_OBJECT

  protected:
    static int _nextId;

    int _id;

    qreal _x, _y, _z;
    qreal _tx, _ty, _tz;
    qreal _rx, _ry, _rz;

    /*!
    * @brief The size of object
    */
    qreal _size;
    Q_PROPERTY(qreal _color READ size WRITE setSize);

    /*!
    * @brief The color of object
    */
    QColor _color;
    Q_PROPERTY(QColor _color READ color WRITE setColor);

    /*!
     * @brief Fuzzy Compare two qreal
     */
    inline bool FuzzyCompare(const qreal p1, const qreal p2)
    {
      return qAbs(p1 - p2) < 0.09;
    }

  public:
    /*!
    * @brief Constructor
    */
    Cube(qreal x, qreal y, qreal z, QObject *parent = NULL);

    /*!
    * @brief Destructor
    */
    virtual ~Cube() {}

    /*!
    * @brief Returns the color of object
    */
    const QColor &color() const { return _color; }

    /*!
    * @brief Sets the color of object
    */
    void setColor(const QColor &color) { _color = color; }

    /*!
    * @brief Draw the object
    */
    void Draw() const;

    /*!
    * @brief Translate the object
    */
    void Translate(qreal tx, qreal ty, qreal tz) { _tx = tx; _ty = ty; _tz = tz; }

    /*!
    * @brief Rotate the object
    */
    void Rotate(qreal rx, qreal ry, qreal rz) { _rx = rx; _ry = ry; _rz = rz; }

    /*!
    * @brief Get the current position
    */
    qreal x() const { return _x; }
    qreal y() const { return _y; }
    qreal z() const { return _z; }

    /*!
    * @brief Get the current translations
    */
    qreal tx() const { return _tx; }
    qreal ty() const { return _ty; }
    qreal tz() const { return _tz; }

    /*!
    * @brief Get the current rotations
    */
    qreal rx() const { return _rx; }
    qreal ry() const { return _ry; }
    qreal rz() const { return _rz; }

    /*!
    * @brief Get size of the object
    */
    qreal size() const { return _size; }

    /*!
    * @brief Set size of the object
    */
    void setSize(const qreal size) { _size = size; }

    /*!
    * @brief Copy position of source cube
    */
    void CopyPosFrom(Cube &source)
    {
      _x = source._x;
      _y = source._y;
      _z = source._z;
    }

    /*!
    * @brief Equality operator
    */
    bool operator==(const Cube &other)
    {
      return FuzzyCompare(_x, other._x) && FuzzyCompare(_y, other._y) && FuzzyCompare(_z, other._z);
    }

  public slots:
     /*!
    * @brief Set the current positions
    */
    void setX(qreal x) { _x = x; }
    void setY(qreal y) { _y = y; }
    void setZ(qreal z) { _z = z; }

    /*!
    * @brief Set the current translations
    */
    void setTx(qreal t) { _tx = t; }
    void setTy(qreal t) { _ty = t; }
    void setTz(qreal t) { _tz = t; }

    /*!
    * @brief Set the current rotations
    */
    void setRx(int r) { _rx = (qreal)r; }
    void setRy(int r) { _ry = (qreal)r; }
    void setRz(int r) { _rz = (qreal)r; }

};

#endif
