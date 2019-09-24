#include "SceneWidget.hpp"

#include "WidgetWindow.hpp"

#include <TestScene/SampleAssetBuilder.hpp>
#include <TestScene/SampleSceneBuilder.hpp>

#include <Ape/CameraSelector.hpp>
#include <Ape/Engine.hpp>
#include <Ape/GLFWGateway.hpp>
#include <Ape/SceneRenderer.hpp>
#include <Ape/StandardInputHandler.hpp>
#include <Ape/StandardShaderProgram.hpp>

class SceneWidget::EngineData
{

public:

    class OpenGLLoader
    {

    public:

        OpenGLLoader(ape::GLFWGateway & gateway)
        {
            gateway.initializeOpenGL();
        }

    };

public:

    explicit EngineData(QOpenGLWidget & widget)
        : loader{gateway}
        , assets{createSampleAssets()}
        , scene{createSampleScene(assets)}
        , cameraSelector{scene}
        , renderer{cameraSelector, shader, {0.0f, 0.0f, 0.0f}}
        , window{widget}
        , inputHandler{window, cameraSelector, 0.1f}
        , engine{window, renderer, inputHandler}
    {
    }

public:

    ape::GLFWGateway gateway;

    OpenGLLoader loader;

    SampleAssetCollection assets;

    SampleScene scene;

    ape::CameraSelector cameraSelector;

    ape::StandardShaderProgram shader;

    ape::SceneRenderer renderer;

    ape::qt::WidgetWindow window;

    ape::StandardInputHandler inputHandler;

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

auto SceneWidget::setFocus(bool const focus)
    -> void
{
    if (!data)
    {
        return;
    }

    auto & cameraManipulator = data->inputHandler.getCameraManipulator();

    if (focus)
    {
        cameraManipulator.activate();

        QWidget::setFocus();
    }
    else
    {
        cameraManipulator.deactivate();
    }
}

// virtual (from QOpenGLWidget)
auto SceneWidget::paintGL()
    -> void
{
    data->engine.processOneFrame();

    update();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::initializeGL()
    -> void
{
    // Do all initialization here rather than inside the constructor

    data = std::make_shared<EngineData>(*this);

    initializeOpenGLFunctions();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::resizeGL(int const width, int const height)
    -> void
{
    glViewport(0, 0, width, height);
}

/*
// virtual (from QOpenGLWidget)
auto SceneWidget::enterEvent(QEvent * const)
    -> void
{
    data->inputHandler.getCameraManipulator().activate();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::leaveEvent(QEvent * const)
    -> void
{
    data->inputHandler.getCameraManipulator().deactivate();
}
*/
