#include "Application.hpp"

#include "Material.hpp"
#include "Square.hpp"
#include "Texture.hpp"
#include "Window.hpp"

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
    {
    }

    auto build()
        -> World
    {
        auto bodies = createBodies();

        auto lighting = createLighting();

        auto camera = createCamera();

        return World{std::move(bodies), std::move(lighting), std::move(camera)};
    }

private:

    auto createBodies() const
        -> std::vector<Body>
    {
        auto bodies = std::vector<Body>{};
        
        createGroundTileBodies(bodies);

        createCubeBodies(bodies);

        createLampBodies(bodies);

        return bodies;
    }

    auto createGroundTileBodies(std::vector<Body> & bodies) const
        -> void
    {
        auto const shape = std::make_shared<Shape>(makeSquare(SquareNormalDirection::outbound));

        auto const material = getGroundMaterial();

        for (auto row = -5; row < +5; ++row)
        {
            for (auto col = -5; col < +5; ++col)
            {
                createGroundTileBody(row, col, shape, material, bodies);
            }
        }
    }

    auto createGroundTileBody(
        int const row,
        int const col,
        std::shared_ptr<Shape> shape,
        Material const & material,
        std::vector<Body> & bodies) const
        -> void
    {
        auto const scaling = glm::scale(glm::mat4{1.0f}, glm::vec3{5.0f, 0.01f, 5.0f});

        auto const position = glm::vec3{row * 5.0f, -2.0f, col * 5.0f};

        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        bodies.emplace_back(std::move(shape), material, *shader, translation * scaling);
    }

    auto getGroundMaterial() const
        -> Material
    {
        auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

        auto const diffuseMapId = makeTexture("ConcreteGround.jpg", GL_RGB);

        auto const specularMapId = makeTexture("ConcreteGround.jpg", GL_RGB);

        auto const shininess = 16.0f;

        return {ambientColor, diffuseMapId, specularMapId, shininess};
    }

    auto createCubeBodies(std::vector<Body> & bodies) const
        -> void
    {
        auto const shape = std::make_shared<Shape>(makeSquare(SquareNormalDirection::outbound));

        auto const positions = getCubeBodyPositions();

        auto const materials = getCubeBodyMaterials();

        for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
        {
            auto const translation = glm::translate(glm::mat4{1.0f}, positions[i]);

            auto const rotation = glm::rotate(
                glm::mat4{1.0f},
                glm::radians(20.0f * i),
                glm::vec3{1.0f, 0.3f, 0.5f});

            auto const & material = materials[i % materials.size()];

            bodies.emplace_back(shape, material, *shader, translation * rotation);
        }
    }

    auto getCubeBodyPositions() const
        -> std::vector<glm::vec3>
    {
        return {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.5f, -3.0f},
            {2.0f, 5.0f, -15.0f},
            {-1.5f, -1.2f, -2.5f},
            {-3.8f, -1.0f, -12.3f},
            {2.4f, -0.4f, -3.5f},
            {-1.7f, 3.0f, -7.5f},
            {1.3f, -1.0f, -2.5f},
            {1.5f, 2.0f, -2.5f},
            {1.5f, 0.2f, -1.5f},
            {-1.3f, 1.0f, -1.5f}};
    }

    auto getCubeBodyMaterials() const
        -> std::vector<Material>
    {
        return {getContainerMaterial()};
    }

    auto getContainerMaterial() const
        -> Material
    {
        auto const ambientColor = glm::vec3{1.0f, 0.5f, 0.31f};

        auto const diffuseMapId = makeTexture("Container.Diffuse.png", GL_RGBA);

        auto const specularMapId = makeTexture("Container.Specular.png", GL_RGBA);

        auto const shininess = 32.0f;

        return {ambientColor, diffuseMapId, specularMapId, shininess};
    }

    auto createLampBodies(std::vector<Body> & bodies) const
        -> void
    {
        auto shape = std::make_shared<Shape>(makeSquare(SquareNormalDirection::inbound));

        auto const positions = getLampBodyPositions();

        auto const scaling = glm::scale(glm::mat4{1.0f}, glm::vec3{0.2f, 0.2f, 0.2f});

        for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
        {
            auto const translation = glm::translate(glm::mat4{1.0f}, positions[i]);

            auto const material = getLampMaterial();

            bodies.emplace_back(shape, material, *shader, translation * scaling);
        }
    }

    auto getLampBodyPositions() const
        -> std::vector<glm::vec3>
    {
        return {
            {0.0f, 0.0f, 5.0f},
            {0.0f, 1.0f, -5.0f}};
    }

    auto getLampMaterial() const
        -> Material
    {
        auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

        auto const diffuseMapId = makeTexture("White.png", GL_RGB);

        auto const specularMapId = makeTexture("White.png", GL_RGB);

        auto const shininess = 32.0f;

        return {ambientColor, diffuseMapId, specularMapId, shininess};
    }

    auto createLighting() const
        -> Lighting
    {
        return {createPointLights(), {}, {}};
    }

    auto createPointLights() const
        -> std::vector<PointLight>
    {
        auto lights = std::vector<PointLight>{};

        const auto positions = getLampBodyPositions();

        for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
        {
            const auto position = positions[i];

            createPointLight(position, lights); 
        }

        return lights;
    }

    auto createPointLight(glm::vec3 const & position, std::vector<PointLight> & pointLights) const
        -> void
    {
        auto const ambient = glm::vec3{0.2f, 0.2f, 0.2f};

        auto const diffuse = glm::vec3{0.8f, 0.5f, 0.7f};

        auto const specular = glm::vec3{1.0f, 0.7f, 0.8f};

        auto const color = Light::Color{ambient, diffuse, specular};

        auto const attenuation = Attenuation{1.0f, 0.09f, 0.032f};

        pointLights.emplace_back(position, attenuation, color);
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

    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);

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

    //setupSpotLights();

    //setupDirectionalLights();
}

auto Application::setupPointLights()
    -> void
{
    auto const numOfPointLights = static_cast<int>(world.lighting.point.size());

    shader.set("lighting.numOfPointLights", numOfPointLights);

    for (auto i = 0; i < numOfPointLights; ++i)
    {
        auto uniformPrefix = "lighting.point[" + std::to_string(i) + "]";

        shader.set(uniformPrefix + ".position", world.lighting.point[i].position);

        shader.set(uniformPrefix + ".color.ambient", world.lighting.point[i].color.ambient);

        shader.set(uniformPrefix + ".color.diffuse", world.lighting.point[i].color.diffuse);

        shader.set(uniformPrefix + ".color.specular", world.lighting.point[i].color.specular);
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
