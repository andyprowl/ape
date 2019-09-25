#include "SceneWidget.hpp"

#include "WidgetWindow.hpp"

#include <TestScene/SampleAssetBuilder.hpp>
#include <TestScene/SampleInputHandler.hpp>
#include <TestScene/SampleSceneBuilder.hpp>

#include <Ape/CameraSelector.hpp>
#include <Ape/Engine.hpp>
#include <Ape/GLFWGateway.hpp>
#include <Ape/SceneRenderer.hpp>
#include <Ape/StandardInputHandler.hpp>
#include <Ape/StandardShaderProgram.hpp>

namespace ape::qt
{

class SceneWidget::EngineHolder
{

public:

    EngineHolder(QOpenGLWidget & widget, SampleScene & scene)
        : cameraSelector{scene}
        , renderer{cameraSelector, shader, {0.0f, 0.0f, 0.0f}}
        , window{widget}
        , inputHandler{window, cameraSelector, shader, scene}
        , engine{window, renderer, inputHandler}
    {
    }

public:

    ape::CameraSelector cameraSelector;

    ape::StandardShaderProgram shader;

    ape::SceneRenderer renderer;

    ape::qt::WidgetWindow window;

    SampleInputHandler inputHandler;

    ape::Engine engine;

};

SceneWidget::SceneWidget(QWidget * const parent)
    : QOpenGLWidget{parent}
{
    // No calls to OpenGL here, do those in initializeGL instead
}

SceneWidget::~SceneWidget()
{
    makeCurrent();

    // Do all resource releasing here

    doneCurrent();
}

auto SceneWidget::start(SampleScene & scene)
    -> void
{
    makeCurrent();

    holder = std::make_shared<EngineHolder>(*this, scene);

    holder->inputHandler.getCameraManipulator().deactivate();
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
auto SceneWidget::initializeGL()
    -> void
{
    // Do all initialization here rather than inside the constructor
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
