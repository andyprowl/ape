#include "Application.hpp"

#include "Window.hpp"
#include "SceneBuilder.hpp"

#include <glm/trigonometric.hpp>

#include <vector>

Application::Application()
    : window{&createWindow("3D Engine", false)}
    , wheelPublisher{*window}
    , keyboardPublisher{*window}
    , shader{"Object.Vertex.glsl", "Object.Fragment.glsl"}
    , scene{createScene(*window, shader)}
    , inputHandler{scene, *window, wheelPublisher, keyboardPublisher, shader}
    , uniforms{shader}
{
    captureMouse();
}

Application::~Application()
{
    glfwTerminate();
}

auto Application::captureMouse() const
    -> void
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

auto Application::run()
    -> void
{
    bindMaterialSamplers();

    while (!wasTerminationRequested())
    {
        processInput();

        render();

        swapBuffers();

        pollEvents();

        updateFrameTime();
    }
}

/*static*/
auto Application::createScene(GLFWwindow & window, ShaderProgram & shader)
    -> Scene
{
    auto builder = SceneBuilder{window, shader};

    return builder.build();
}

auto Application::bindMaterialSamplers() const
    -> void
{
    shader.use();

    shader.getUniform<int>("material.diffuse") = 0;

    shader.getUniform<int>("material.specular") = 1;
}

auto Application::wasTerminationRequested() const
    -> bool
{
    return glfwWindowShouldClose(window);
}

auto Application::processInput()
    -> void
{
    const auto lastFrameDuration = timeTracker.getLastFrameDuration();

    inputHandler.processInput(lastFrameDuration);
}

auto Application::render()
    -> void
{
    if (!isWindowReady())
    {
        return;
    }

    clear();

    setupCamera();

    setupLights();

    drawScene();
}

auto Application::isWindowReady() const
    -> bool
{
    auto const size = getWindowSize(*window);

    return (size.height > 0);
}

auto Application::clear()
    -> void
{
    auto const backgroundColor = glm::vec3{0.0f, 0.0f, 0.0f};

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

auto Application::setupCamera()
    -> void
{
    auto const aspectRatio = getWindowRatio(*window);

    scene.camera.setAspectRatio(aspectRatio);

    shader.use();

    uniforms.cameraPosition.set(scene.camera.getPosition());

    uniforms.cameraTransformation.set(scene.camera.getTransformation());
}

auto Application::setupLights()
    -> void
{
    shader.use();

    uniforms.lighting.set(scene.lighting);
}

auto Application::drawScene()
    -> void
{
    for (auto & mesh : scene.bodies)
    {
        mesh.draw();
    }
}

auto Application::swapBuffers()
    -> void
{
    glfwSwapBuffers(window);
}

auto Application::pollEvents()
    -> void
{
    glfwPollEvents();
}

auto Application::updateFrameTime()
    -> void
{
    timeTracker.update();
}
