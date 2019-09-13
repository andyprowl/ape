#include "application.hpp"

#include "material.hpp"
#include "square.hpp"
#include "texture.hpp"
#include "window.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace
{

class WorldBuilder
{

public:

    WorldBuilder(GLFWwindow & window, ShaderProgram const & shader)
        : shader{&shader}
        , window{&window}
        , objectPositions{makeObjectPositions()}
        , materials{makeMaterials()}
    {
    }

    auto build()
        -> World
    {
        auto widgets = createWidgets();

        auto light = createLight(widgets.back());

        auto camera = createCamera();

        return World{std::move(widgets), std::move(light), std::move(camera)};
    }

private:

    auto createWidgets() const
        -> std::vector<Widget>
    {
        auto widgets = makeCubeObjects();

        widgets.push_back(makeLamp());

        return widgets;
    }

    auto makeObjectPositions() const
        -> std::vector<glm::vec3>
    {
        return {
            {0.0f, 0.0f, 0.0f},
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

    auto makeMaterials() const
        -> std::vector<Material>
    {
        auto const emerald = Material{
            {0.0215f, 0.1745f, 0.0215f},
            {0.07568f, 0.61424f, 0.07568f},
            {0.633f, 0.727811f, 0.633f},
            32.0f};

        auto const redPlastic = Material{
            {0.0f, 0.0f, 0.0f},
            {0.5f, 0.0f, 0.0f},
            {0.7f, 0.6f, 0.6f},
            256.0f};
        
        auto const greenRubber = Material{
            {0.0f, 0.05f, 0.0f},
            {0.4f, 0.5f, 0.4f},
            {0.04f, 0.7f, 0.04f},
            64.0f};
        
        auto const cyanPlastic = Material{
            {0.0f, 0.1f, 0.06f},
            {0.0f, 0.50980392f, 0.50980392f},
            {0.50196078f , 0.50196078f, 0.50196078f},
            16.0f};

        auto const custom = Material{
            {1.0f, 0.5f, 0.31f},
            {1.0f, 0.5f, 0.31f},
            {0.5f, 0.5f, 0.5f},
            32.0f};

        return {emerald, redPlastic, greenRubber, cyanPlastic, custom};
    }

    auto makeCubeObjects() const
        -> std::vector<Widget>
    {
        auto const shape = std::make_shared<Shape>(makeSquare());

        auto const containerTextureId = makeTexture("container.jpg", GL_RGB);

        auto const awesomeTextureId = makeTexture("awesomeface.png", GL_RGBA);

        auto const textureIds = std::vector<int>{containerTextureId, awesomeTextureId};

        auto widgets = std::vector<Widget>{};

        for (auto i = 0; i < static_cast<int>(objectPositions.size()); ++i)
        {
            auto const translation = glm::translate(glm::mat4{1.0f}, objectPositions[i]);

            auto const rotation = glm::rotate(
                glm::mat4{1.0f},
                glm::radians(20.0f * i),
                glm::vec3{1.0f, 0.3f, 0.5f});

            widgets.emplace_back(
                shape,
                textureIds,
                materials[i % materials.size()],
                *shader,
                translation * rotation);
        }

        return widgets;
    }

    auto makeLamp() const
        -> Widget
    {
        auto shape = std::make_unique<Shape>(makeSquare());

        auto const textureId = makeTexture("white.png", GL_RGBA);

        auto const scaling = glm::scale(glm::mat4{1.0f}, glm::vec3{0.2f, 0.2f, 0.2f});

        auto const translation = glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, 0.0f, 2.0f});

        return Widget{
            std::move(shape),
            {textureId, textureId},
            materials[0],
            *shader,
            translation * scaling};
    }

    auto createLight(Widget & body) const
        -> Light
    {
        const auto position = body.getPosition();

        auto const ambient = glm::vec3{0.2f, 0.2f, 0.2f};

        auto const diffuse = glm::vec3{0.5f, 0.5f, 0.5f};

        auto const specular = glm::vec3{1.0f, 1.0f, 1.0f};

        return {position, ambient, diffuse, specular};
    }

    auto createCamera() const
        -> Camera
    {
        auto const position = glm::vec3{0.0f, 0.0f, 3.0f};

        auto const direction = glm::vec3{0.0f, 0.0f, -1.0f};

        auto const up = glm::vec3{0.0f, 1.0f, 0.0f};

        auto const fieldOfView = glm::radians(45.0f);

        auto const aspectRatio = getWindowRatio(*window);

        return {position, direction, up, fieldOfView, aspectRatio};
    }

private:

    ShaderProgram const * shader;

    GLFWwindow * window;

    std::vector<glm::vec3> objectPositions;

    std::vector<Material> materials;

};

} // unnamed namespace

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
    auto program = ShaderProgram{"object.vertex.glsl", "object.fragment.glsl"};

    program.use();

    program.set("textureWeight", 0.5f);

    program.set("colorWeight", 0.5f);

    program.bindFragmentSamplers({"texSampler1", "texSampler2"});

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
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

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

    shader.set("light.position", world.light.position);

    shader.set("light.ambient", world.light.ambient);

    shader.set("light.diffuse", world.light.diffuse);

    shader.set("light.specular", world.light.specular);
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
