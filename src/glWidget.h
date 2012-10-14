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
    int _rxa;
    int _rya;
    int _moveA;
    int _moveB;
    int _moveX;
    int _moveY;
    int _moveZ;
    int _toAdd;
    int _score;
    Axe _axeA;
    Axe _axeB;
    Cube   *_plateau;
    Cube   *_food;
    QHud   *_debugHud;
    QHud   *_gameHud;
    QTimer _refreshTimer;
    QTimer _fpsTimer;
    QTimer _gameTimer;
    QTimer _cameraTimer;
    QVector<Cube*> _items;
    QPoint _lastPos;

  public:
    GlWidget(QWidget *parent = NULL);
    virtual ~GlWidget();

    QSize sizeHint() const;

    static int normalizeAngle(int angle);

  protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void convertMove();
    void rotateCube(Axe endAxe);
    void draw();

  protected slots:
    void updateFPS();
    void eatFood();
    void processGame();
    void updateCamera();
};

#endif
