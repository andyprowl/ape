#include "Application.hpp"

#include "SampleAssetBuilder.hpp"
#include "CameraDrivenPipeline.hpp"
#include "CameraSpotlightSynchronizer.hpp"
#include "SampleSceneBuilder.hpp"
#include "Window.hpp"

#include "GLFW.hpp"

Application::Application()
    : window{"APE 3D Engine", false}
    , shader{createShader()}
    , assets{createAssets()}
    , scene{createScene(assets)}
    , renderer{shader, {0.0f, 0.0f, 0.0f}}
    , inputHandler{scene, window, shader}
    , rateTracker{timeTracker, 500}
    , resizeHandlerConnection{registerWindowResizeHandler()}
{
    window.captureMouse();
}

Application::~Application()
{
    glfwTerminate();
}

auto Application::run()
    -> void
{
    setViewport();

    while (!wasTerminationRequested())
    {
        processInput();

        render();

        recordFrameDuration();

        reportFramesPerSecond();
    }
}

/* static */
auto Application::createShader()
    -> ShaderProgram
{
    auto shader = ShaderProgram{"Object.Vertex.glsl", "Object.Fragment.glsl"};

    shader.use();

    shader.getUniform<int>("material.diffuse") = 0;

    shader.getUniform<int>("material.specular") = 1;

    return shader;
}

/* static */
auto Application::createAssets()
    -> SampleAssetCollection
{
    auto const builder = SampleAssetBuilder{};

    return builder.build();
}

/* static */
auto Application::createScene(SampleAssetCollection & assets)
    -> SampleScene
{
    auto const builder = SampleSceneBuilder{assets};

    return builder.build();
}

auto Application::registerWindowResizeHandler()
    -> ScopedSignalConnection
{
    return window.onResize.registerHandler([this] (Size<int> const & /*newSize*/)
    {
        setViewport();
    });
}

auto Application::setViewport()
    -> void
{
    auto const size = window.getSize();

    glViewport(0, 0, size.width, size.height);

    auto const aspectRatio = window.getAspectRatio();

    scene.cameraSystem.activeCamera->setAspectRatio(aspectRatio);
}

auto Application::wasTerminationRequested() const
    -> bool
{
    return window.shouldClose();
}

auto Application::processInput()
    -> void
{
    glfwPollEvents();

    auto const lastFrameDuration = timeTracker.getLastFrameDuration();

    inputHandler.processInput(lastFrameDuration);
}

auto Application::render()
    -> void
{
    if (!isWindowReady())
    {
        return;
    }

    renderer.render(scene);

    window.swapBuffers();
}

auto Application::isWindowReady() const
    -> bool
{
    auto const size = window.getSize();

    return (size.height > 0);
}

auto Application::recordFrameDuration()
    -> void
{
    timeTracker.update();
}

auto Application::reportFramesPerSecond()
    -> void
{
    rateTracker.update();
}
