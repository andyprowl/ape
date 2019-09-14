#include "Application.hpp"

#include "Window.hpp"
#include "WorldBuilder.hpp"

#include <glm/trigonometric.hpp>

#include <vector>

Application::Application()
    : window{&createWindow()}
    , shader{createShaderProgram()}
    , world{createWorld(*window, shader)}
    , inputHandler{world, *window, shader}
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
auto Application::createShaderProgram()
    -> ShaderProgram
{
    auto program = ShaderProgram{"Object.Vertex.glsl", "Object.Fragment.glsl"};

    program.use();

    program.bindFragmentSamplers({"material.diffuse", "material.specular"});

    return program;
}

/*static*/
auto Application::createWorld(GLFWwindow & window, ShaderProgram const & shader)
    -> World
{
    auto builder = WorldBuilder{window, shader};

    return builder.build();
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
    clear();

    setupCamera();

    setupLights();

    drawWorld();
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
    world.camera.setAspectRatio(getWindowRatio(*window));

    auto const view = world.camera.getView();

    auto const proj = world.camera.getProjection();

    shader.use();

    shader.set("viewPosition", world.camera.getPosition());

    shader.set("transform.view", view);

    shader.set("transform.proj", proj);
}

auto Application::setupLights()
    -> void
{
    shader.use();

    setupPointLights();

    setupSpotLights();

    setupDirectionalLights();
}

auto Application::setupPointLights()
    -> void
{
    auto const numOfPointLights = static_cast<int>(world.lighting.point.size());

    shader.set("lighting.numOfPointLights", numOfPointLights);

    for (auto i = 0; i < numOfPointLights; ++i)
    {
        auto const & light = world.lighting.point[i];

        auto uniformPrefix = "lighting.point[" + std::to_string(i) + "]";

        shader.set(uniformPrefix + ".position", light.position);

        shader.set(uniformPrefix + ".color.ambient", light.color.ambient);

        shader.set(uniformPrefix + ".color.diffuse", light.color.diffuse);

        shader.set(uniformPrefix + ".color.specular", light.color.specular);

        shader.set(uniformPrefix + ".attenuation.constant", light.attenuation.constant);

        shader.set(uniformPrefix + ".attenuation.linear", light.attenuation.linear);

        shader.set(uniformPrefix + ".attenuation.quadratic", light.attenuation.quadratic);
    }
}

auto Application::setupSpotLights()
    -> void
{
    auto const numOfSpotLights = static_cast<int>(world.lighting.spot.size());

    shader.set("lighting.numOfSpotLights", numOfSpotLights);

    for (auto i = 0; i < numOfSpotLights; ++i)
    {
        auto const & light = world.lighting.spot[i];

        auto uniformPrefix = "lighting.spot[" + std::to_string(i) + "]";

        shader.set(uniformPrefix + ".position", light.position);

        shader.set(uniformPrefix + ".direction", light.direction);

        shader.set(uniformPrefix + ".innerCutoffCosine", glm::cos(light.cutoff.inner));

        shader.set(uniformPrefix + ".outerCutoffCosine", glm::cos(light.cutoff.outer));

        shader.set(uniformPrefix + ".color.ambient", light.color.ambient);

        shader.set(uniformPrefix + ".color.diffuse", light.color.diffuse);

        shader.set(uniformPrefix + ".color.specular", light.color.specular);

        shader.set(uniformPrefix + ".attenuation.constant", light.attenuation.constant);

        shader.set(uniformPrefix + ".attenuation.linear", light.attenuation.linear);

        shader.set(uniformPrefix + ".attenuation.quadratic", light.attenuation.quadratic);
    }
}

auto Application::setupDirectionalLights()
    -> void
{
    auto const numOfDirectionalLights = static_cast<int>(world.lighting.directional.size());

    shader.set("lighting.numOfDirectionalLights", numOfDirectionalLights);

    for (auto i = 0; i < numOfDirectionalLights; ++i)
    {
        auto const & light = world.lighting.directional[i];

        auto uniformPrefix = "lighting.directional[" + std::to_string(i) + "]";

        shader.set(uniformPrefix + ".direction", light.direction);

        shader.set(uniformPrefix + ".color.ambient", light.color.ambient);

        shader.set(uniformPrefix + ".color.diffuse", light.color.diffuse);

        shader.set(uniformPrefix + ".color.specular", light.color.specular);
    }
}

auto Application::drawWorld()
    -> void
{
    for (auto & body : world.bodies)
    {
        body.draw();
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
