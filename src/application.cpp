#include "application.hpp"

#include "square.h"
#include "texture.hpp"
#include "window.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace
{

class WorldBuilder
{

public:

    explicit WorldBuilder(ShaderProgram const & shader)
        : shader{&shader}
    {
    }

    auto build(glm::vec3 const & objectColor)
        -> World
    {
        auto widgets = makeCubeObjects(objectColor);

        widgets.push_back(makeLamp());

        return World{std::move(widgets)};
    }

private:

    auto makeCubeObjects(glm::vec3 const & objectColor) const
        -> std::vector<Widget>
    {
        auto const shape = std::make_shared<Shape>(makeSquare(objectColor));

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

            widgets.emplace_back(shape, textureIds, *shader, translation * rotation);
        }

        return widgets;
    }

    auto makeWidgetPositions() const
        -> std::vector<glm::vec3>
    {
        return {
            {0.0f, 0.5f, -3.0f},
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

    auto makeLamp() const
        -> Widget
    {
        auto shape = std::make_unique<Shape>(makeSquare(glm::vec3{1.0f, 1.0f, 1.0f}));

        auto const textureId = makeTexture("white.png", GL_RGBA);

        auto const scaling = glm::scale(glm::mat4{1.0f}, glm::vec3{0.2f, 0.2f, 0.2f});

        auto const translation = glm::translate(glm::mat4{1.0f}, glm::vec3{-2.0f, -1.0f, 0.0f});

        return Widget{std::move(shape), {textureId, textureId}, *shader, translation * scaling};
    }

private:

    ShaderProgram const * shader;

};

} // unnamed namespace

Application::Application()
    : window{&createWindow()}
    , shader{createShaderProgram()}
    , world{createWorld(shader)}
    , camera{createCamera(*window)}
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
    auto program = ShaderProgram{"object.vertex.glsl", "object.fragment.glsl"};

    program.use();

    program.set("textureWeight", 0.5f);

    program.set("colorWeight", 0.5f);

    program.bindFragmentSamplers({"texSampler1", "texSampler2"});

    return program;
}

/*static*/
auto Application::createWorld(ShaderProgram const & shader)
    -> World
{
    auto builder = WorldBuilder{shader};

    auto const objectColor = glm::vec3{1.0f, 0.5f, 0.31f};

    return builder.build(objectColor);
}

/*static*/
auto Application::createCamera(GLFWwindow & window)
    -> Camera
{
    auto const position = glm::vec3{0.0f, 0.0f, 3.0f};

    auto const direction = glm::vec3{0.0f, 0.0f, -1.0f};

    auto const up = glm::vec3{0.0f, 1.0f, 0.0f};

    auto const fieldOfView = glm::radians(45.0f);

    auto const aspectRatio = getWindowRatio(window);

    return {position, direction, up, fieldOfView, aspectRatio};
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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

auto Application::setupCamera()
    -> void
{
    camera.setAspectRatio(getWindowRatio(*window));

    auto const view = camera.getView();

    auto const proj = camera.getProjection();

    shader.use();

    shader.set("view", view);

    shader.set("proj", proj);
}

auto Application::setupLights()
    -> void
{
    shader.use();

    shader.set("lightColor", glm::vec3{1.0f, 1.0f, 1.0f});

    const auto model = world.widgets.back().getModelTransformation();
    
    const auto position = glm::vec3{model[3]};

    shader.set("lightPosition", position);
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
