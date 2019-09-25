#include "SceneWidget.hpp"

#include "WidgetWindow.hpp"

#include <TestScene/SampleAssetBuilder.hpp>
#include <TestScene/SampleInputHandler.hpp>
#include <TestScene/SampleSceneBuilder.hpp>

#include <Ape/CameraSelector.hpp>
#include <Ape/Engine.hpp>
#include <Ape/SceneRenderer.hpp>
#include <Ape/StandardShaderProgram.hpp>

namespace ape::qt
{

class SceneWidget::EngineHolder
{

public:

    EngineHolder(QOpenGLWidget & widget, SampleScene & scene, RenderingContext const & context)
        : window{widget}
        , cameraSelector{scene}
        , renderer{context, cameraSelector, shader, {0.0f, 0.0f, 0.0f}}
        , inputHandler{window, cameraSelector, shader, scene}
        , engine{window, renderer, inputHandler}
    {
    }

public:

    ape::qt::WidgetWindow window;

    ape::CameraSelector cameraSelector;

    ape::StandardShaderProgram shader;

    ape::SceneRenderer renderer;

    SampleInputHandler inputHandler;

    ape::Engine engine;

};

SceneWidget::SceneWidget(ape::RenderingContext const & renderingContext, QWidget * const parent)
    : QOpenGLWidget{parent}
    , renderingContext{renderingContext}
{
}

SceneWidget::~SceneWidget()
{
    makeCurrent();

    // Do all resource releasing here

    doneCurrent();
}

auto SceneWidget::engage(SampleScene & scene)
    -> void
{
    makeCurrent();

    holder = std::make_shared<EngineHolder>(*this, scene, renderingContext);

    holder->inputHandler.getCameraManipulator().deactivate();
}

auto SceneWidget::getCameraSelector()
    -> CameraSelector &
{
    if (holder == nullptr)
    {
        throw SceneWidgetNotEngaged{};
    }

    return holder->cameraSelector;
}

// virtual (from QOpenGLWidget)
auto SceneWidget::paintGL()
    -> void
{
    if (holder == nullptr)
    {
        return;
    }

    holder->engine.processOneFrame();

    update();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::focusInEvent(QFocusEvent * const)
    -> void
{
    holder->inputHandler.getCameraManipulator().activate();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::focusOutEvent(QFocusEvent * const)
    -> void
{
    holder->inputHandler.getCameraManipulator().deactivate();
}

} // namespace ape::qt
