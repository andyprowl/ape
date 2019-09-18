#include "Application.hpp"

#include "CameraDrivenPipeline.h"
#include "CameraSpotlightSynchronizer.h"
#include "SceneBuilder.hpp"
#include "Window.hpp"

#include "GLFW.hpp"

#include <glm/trigonometric.hpp>

#include <vector>

Application::Application()
    : window{"APE 3D Engine", false}
    , shader{createShader()}
    , scene{createScene(window)}
    , renderer{shader, {0.0f, 0.0f, 0.0f}}
    , inputHandler{scene, window, shader}
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
    auto const synchronizer = CameraSpotlightSynchronizer{scene.camera, scene.lighting.spot.back()};

    setViewport();

    while (!wasTerminationRequested())
    {
        processInput();

        render();

        recordFrameDuration();
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
auto Application::createScene(Window const & window)
    -> Scene
{
    auto builder = SceneBuilder{window};

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

    scene.camera.setAspectRatio(aspectRatio);
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
