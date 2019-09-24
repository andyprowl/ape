#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

class SceneWidget : public QOpenGLWidget, public QOpenGLFunctions
{

    Q_OBJECT;

public:

    explicit SceneWidget(QWidget * parent);

    ~SceneWidget();

    auto setFocus(bool focus)
        -> void;

private:

    // virtual (from QOpenGLWidget)
    auto paintGL()
        -> void override;

    // virtual (from QOpenGLWidget)
    auto initializeGL()
        -> void override;

    // virtual (from QOpenGLWidget)
    auto resizeGL(int width, int height)
        -> void override;

    /*
    // virtual (from QOpenGLWidget)
    auto enterEvent(QEvent * e)
        -> void override;

    // virtual (from QOpenGLWidget)
    auto leaveEvent(QEvent * e)
        -> void override;
    */

private:

    class EngineData;

private:

    std::shared_ptr<EngineData> data;

};