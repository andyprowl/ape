#pragma once

#include <Ape/RenderingContext.hpp>

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

class SampleScene;

namespace ape
{

class CameraSelector;

} // namespace ape

namespace ape::qt
{

class SceneWidgetNotEngaged : public std::logic_error
{

public:

    SceneWidgetNotEngaged()
        : logic_error{"The widget is not associated to a scene"}
    {
    }

};

class SceneWidget : public QOpenGLWidget
{

    Q_OBJECT;

public:

    SceneWidget(ape::RenderingContext const & renderingContext, QWidget * parent);

    ~SceneWidget();

    auto engage(SampleScene & scene)
        -> void;

    auto getCameraSelector()
        -> CameraSelector &;

private:

    // virtual (from QOpenGLWidget)
    auto paintGL()
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

    ape::RenderingContext renderingContext;

};

} // namespace ape::qt
