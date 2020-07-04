#pragma once

#include <QWidget>
#include <QOpenGLWidget>

#include <memory>

class OpenGLWidget : public QOpenGLWidget
{
public:
    OpenGLWidget(QWidget* parent = 0);
    ~OpenGLWidget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    std::unique_ptr<class Renderer> _renderer;

    bool _firstClick = true;
    int _lastMouseX;
    int _lastMouseY;
};
