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
        , inputHandler{window, cameraSelector, shader, scene}
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

    data->inputHandler.getCameraManipulator().deactivate();

    initializeOpenGLFunctions();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::focusInEvent(QFocusEvent * const)
    -> void
{
    data->inputHandler.getCameraManipulator().activate();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::focusOutEvent(QFocusEvent * const)
    -> void
{
    data->inputHandler.getCameraManipulator().deactivate();
}

} // namespace ape::qt
