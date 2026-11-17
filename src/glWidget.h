#ifndef __GLWIDGET_H
#define __GLWIDGET_H

#include <QOpenGLFunctions_2_1>
#include <QOpenGLWidget>
#include <QPoint>
#include <QTimer>
#include <QVector>

class Cube;
class QHud;

class GlWidget : public QOpenGLWidget, protected QOpenGLFunctions_2_1
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
    GlWidget(QWidget *parent = nullptr);
    virtual ~GlWidget();

    QSize sizeHint() const override;

    static int normalizeAngle(int angle);

  protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
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
