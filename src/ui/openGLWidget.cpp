#include "openGLWidget.h"

#include <QKeyEvent>

#include "renderer/Renderer.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    // From the Qt documentation:
    //   > If mouse tracking is switched off, mouse move events only occur if a mouse button is pressed while the mouse is being moved.
    //   > If mouse tracking is switched on, mouse move events occur even if no mouse button is pressed.
    this->setMouseTracking(false);

    _renderer = std::make_unique<Renderer>();
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::initializeGL()
{
    QOpenGLWidget::initializeGL();

    _renderer->init();
}

void OpenGLWidget::paintGL()
{
    QOpenGLWidget::paintGL();

    _renderer->render();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);

    _renderer->update(w, h);
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    float delta = 0.01f;
    switch (event->key()) {
    case Qt::Key_W:
        _renderer->MoveCamera(CameraMovement::FORWARD, delta);
        break;
    case Qt::Key_A:
        _renderer->MoveCamera(CameraMovement::LEFT, delta);
        break;
    case Qt::Key_S:
        _renderer->MoveCamera(CameraMovement::BACKWARD, delta);
        break;
    case Qt::Key_D:
        _renderer->MoveCamera(CameraMovement::RIGHT, delta);
        break;
    case Qt::Key_Q:
        _renderer->MoveCamera(CameraMovement::DOWN, delta);
        break;
    case Qt::Key_E:
        _renderer->MoveCamera(CameraMovement::UP, delta);
        break;
    }

    // Update the widget to call paintGL and so draw again
    update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    int xOffset;
    int yOffset;

    if (_firstClick)
    {
        _lastMouseX = event->x();
        _lastMouseY = event->y();
        _firstClick = false;
    }

    xOffset = _lastMouseX - event->x();
    yOffset = _lastMouseY - event->y();

    _renderer->RotateCamera(-1 * (float)xOffset, (float)yOffset);

    _lastMouseX = event->x();
    _lastMouseY = event->y();

    // Update the widget to call paintGL and so draw again
    update();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    _firstClick = true;

    event->accept();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    /**
     *  angleDelta() returns the relative amount that the wheel was rotated, in eighths of a degree.
     *  A positive value indicates that the wheel was rotated forwards away from the user.
     *  
     *  angleDelta().y() provides the angle through which the common vertical mouse wheel was rotated since the previous event.
     */
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        _renderer->ZoomCamera((float)numSteps.y());
    }

    event->accept();

    // Update the widget to call paintGL and so draw again
    update();
}
