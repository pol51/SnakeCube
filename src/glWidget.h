#ifndef __GLWIDGET_H
#define __GLWIDGET_H

#include <QGLWidget>
#include <QTimer>

class Cube;
class QHud;

class GlWidget : public QGLWidget
{
  Q_OBJECT

  protected:
    enum Axe
    {
      eXn, eXp, eYn, eYp, eZn, eZp
    };

    static QString _axeName[6];

  protected:
    QTimer _refreshTimer;
    QTimer _fpsTimer;
    QTimer _gameTimer;
    QTimer _cameraTimer;
    Cube   *_plateau;
    QHud   *_gameHud;
    QVector<Cube*> _items;
    QPoint _lastPos;
    int _fps;
    int _rx;
    int _ry;
    int _rz;
    int _rxg;
    int _ryg;
    int _rzg;
    int _rxgc;
    int _rygc;
    int _rzgc;
    int _moveA;
    int _moveB;
    int _moveX;
    int _moveY;
    int _moveZ;
    int _toAdd;
    Axe _axeA;
    Axe _axeB;

  public:
    GlWidget(QWidget *parent = NULL);
    virtual ~GlWidget();

    QSize sizeHint() const;

    static int normalizeAngle(int angle);

  protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int width, int height);
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    void convertMove();
    void rotateCube(Axe endAxe);
    virtual void draw();

  protected slots:
    void updateFPS();
    void processGame();
    void updateCamera();
};

#endif
