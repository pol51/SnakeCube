#ifndef __GLWIDGET_H
#define __GLWIDGET_H

#include <QGLWidget>
#include <QTimer>

class Cube;

class GlWidget : public QGLWidget
{
  Q_OBJECT

  protected:
  enum Axe
  {
    eXn, eXp, eYn, eYp, eZn, eZp,
  };

  protected:
    QTimer _refreshTimer;
    QTimer _fpsTimer;
    QTimer _gameTimer;
    QVector<Cube*> _plateau;
    QVector<Cube*> _items;
    QPoint _lastPos;
    int _fps;
    int _rx;
    int _ry;
    int _rz;
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
    const QString AxeName(Axe axe);
    void rotateCube(Axe endAxe);
    virtual void draw();

  protected slots:
    void updateFPS();
    void processGame();

  signals:
    void FPS(int);
    void titleFPS(const QString);
};

#endif
