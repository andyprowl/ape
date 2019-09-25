#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

class SampleScene;

namespace ape::qt
{

class SceneWidget : public QOpenGLWidget
{

    Q_OBJECT;

public:

    explicit SceneWidget(QWidget * parent);

    ~SceneWidget();

    auto start(SampleScene & scene)
        -> void;

private:

    // virtual (from QOpenGLWidget)
    auto paintGL()
        -> void override;

    // virtual (from QOpenGLWidget)
    auto initializeGL()
        -> void override;

    // virtual (from QOpenGLWidget)
    auto focusInEvent(QFocusEvent * const)
        -> void override;

    // virtual (from QOpenGLWidget)
    auto focusOutEvent(QFocusEvent * const)
        -> void override;

private:

    class EngineHolder;

private:

    std::shared_ptr<EngineHolder> holder;

    QOpenGLContext * sharedContext;

};

} // namespace ape::qt
