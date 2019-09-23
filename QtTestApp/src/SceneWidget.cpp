#include "SceneWidget.hpp"

#include "WidgetWindow.hpp"

#include <TestScene/SampleAssetBuilder.hpp>
#include <TestScene/SampleSceneBuilder.hpp>

#include <Ape/GLFWGateway.hpp>

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
        , window{widget}
        , assets{createSampleAssets()}
        , scene{createSampleScene(assets)}
        , renderer{scene, shader, {0.0f, 0.0f, 0.0f}}
        , inputHandler{window, scene, 0.1f}
        , engine{window, renderer, inputHandler}
    {
    }

public:

    ape::GLFWGateway gateway;

    OpenGLLoader loader;

    ape::qt::WidgetWindow window;

    SampleAssetCollection assets;

    SampleScene scene;

    ape::StandardShaderProgram shader;

    ape::SceneRenderer renderer;

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
