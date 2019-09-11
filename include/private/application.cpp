#include "application.h"

#include "texture.hpp"
#include "window.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace
{

class WorldBuilder
{

public:

    explicit WorldBuilder(ShaderProgram const & shaderProgram)
        : shaderProgram{&shaderProgram}
    {
    }

    auto build()
        -> World
    {
        auto const shape = std::make_shared<Shape>(makeSquare());

        auto const containerTextureId = makeTexture("container.jpg", GL_RGB);

        auto const awesomeTextureId = makeTexture("awesomeface.png", GL_RGBA);

        auto const textureIds = std::vector<int>{containerTextureId, awesomeTextureId};

        auto const positions = makeWidgetPositions();

        auto widgets = std::vector<Widget>{};

        for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
        {
            auto const translation = glm::translate(glm::mat4{1.0f}, positions[i]);

            auto const rotation = glm::rotate(
                glm::mat4{1.0f},
                glm::radians(20.0f * i),
                glm::vec3{1.0f, 0.3f, 0.5f});

            widgets.emplace_back(shape, textureIds, *shaderProgram, translation * rotation);
        }

        return World{std::move(widgets)};
    }

private:

    auto makeWidgetPositions() const
        -> std::vector<glm::vec3>
    {
        return {
            {0.0f, 0.0f,  0.0f},
            {2.0f, 5.0f, -15.0f},
            {-1.5f, -2.2f, -2.5f},
            {-3.8f, -2.0f, -12.3f},
            {2.4f, -0.4f, -3.5f},
            {-1.7f, 3.0f, -7.5f},
            {1.3f, -2.0f, -2.5f},
            {1.5f, 2.0f, -2.5f},
            {1.5f, 0.2f, -1.5f},
            {-1.3f, 1.0f, -1.5f}};
    }

private:

    ShaderProgram const * shaderProgram;

};

} // unnamed namespace

Application::Application()
    : window{&createWindow()}
    , shader{createShaderProgram()}
    , world{buildWorld(shader)}
    , camera{{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}}
    , inputHandler{world, camera, *window, shader}
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
    auto program = ShaderProgram{"vertex.glsl", "fragment.glsl"};

    program.use();

    program.set("weight", 0.5f);

    program.set("colorWeight", 0.5f);

    program.set("transform", glm::mat4{1.0f});

    program.bindFragmentSamplers({"texSampler1", "texSampler2"});

    return program;
}

/*static*/
auto Application::buildWorld(ShaderProgram const & program)
    -> World
{
    auto builder = WorldBuilder{program};

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

    drawWorld();
}

auto Application::clear()
    -> void
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

auto Application::setupCamera()
    -> void
{
    auto const view = camera.getView();

    auto const fov = glm::radians(45.0f);

    auto const proj = glm::perspective(fov, getWindowRatio(*window), 0.1f, 100.0f);

    shader.set("view", view);

    shader.set("proj", proj);
}

auto Application::drawWorld()
    -> void
{
    for (auto & widget : world.widgets)
    {
        widget.draw();
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
