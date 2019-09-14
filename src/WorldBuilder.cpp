#include "WorldBuilder.hpp"

#include "Square.hpp"
#include "Window.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

WorldBuilder::WorldBuilder(GLFWwindow & window, ShaderProgram const & shader)
    : shader{&shader}
    , window{&window}
{
}

auto WorldBuilder::build() const
    -> World
{
    auto bodies = createBodies();

    auto lighting = createLighting();

    auto camera = createCamera();

    return World{std::move(bodies), std::move(lighting), std::move(camera)};
}

auto WorldBuilder::createBodies() const
    -> std::vector<Body>
{
    auto bodies = std::vector<Body>{};
        
    createCubeBodies(bodies);

    createGroundTileBodies(bodies);

    createFlashLightBodies(bodies);

    createLampBodies(bodies);

    return bodies;
}

auto WorldBuilder::createGroundTileBodies(std::vector<Body> & bodies) const
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

auto WorldBuilder::createGroundTileBody(
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

auto WorldBuilder::getGroundMaterial() const
    -> Material
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto diffuseMap = loadTexture("ConcreteGround.jpg");

    auto specularMap = loadTexture("ConcreteGround.jpg");

    auto const shininess = 16.0f;

    return {ambientColor, std::move(diffuseMap), std::move(specularMap), shininess};
}

auto WorldBuilder::createCubeBodies(std::vector<Body> & bodies) const
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

auto WorldBuilder::getCubeBodyPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.5f, -3.0f},
        {2.0f, 5.0f, -15.0f},
        {-1.5f, -1.0f, -2.5f},
        {-3.8f, -1.0f, -12.3f},
        {2.4f, -0.4f, -3.5f},
        {-1.7f, 3.0f, -7.5f},
        {1.3f, -1.0f, -2.5f},
        {1.5f, 2.0f, -2.5f},
        {1.5f, 0.2f, -1.5f},
        {-1.3f, 1.0f, -1.5f}};
}

auto WorldBuilder::getCubeBodyMaterials() const
    -> std::vector<Material>
{
    return {getContainerMaterial()};
}

auto WorldBuilder::getContainerMaterial() const
    -> Material
{
    auto const ambientColor = glm::vec3{1.0f, 0.5f, 0.31f};

    auto const diffuseMapId = loadTexture("Container.Diffuse.png");

    auto const specularMapId = loadTexture("Container.Specular.png");

    auto const shininess = 32.0f;

    return {ambientColor, diffuseMapId, specularMapId, shininess};
}

auto WorldBuilder::createLampBodies(std::vector<Body> & bodies) const
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

auto WorldBuilder::getLampBodyPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {0.0f, 0.0f, 5.0f},
        {0.0f, 1.0f, -5.0f}};
}

auto WorldBuilder::getLampMaterial() const
    -> Material
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const diffuseMapId = loadTexture("White.png");

    auto const specularMapId = loadTexture("White.png");

    auto const shininess = 32.0f;

    return {ambientColor, diffuseMapId, specularMapId, shininess};
}

auto WorldBuilder::createFlashLightBodies(std::vector<Body> & bodies) const
    -> void
{
    auto shape = std::make_shared<Shape>(makeSquare(SquareNormalDirection::inbound));

    auto const positions = getFlashLightBodyPositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, glm::vec3{0.3f, 0.1f, 0.1f});

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const position = positions[i];

        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        auto const rotation = computeFlashLightBodyRotationMatrix(position);

        auto const material = getFlashLightMaterial();

        bodies.emplace_back(shape, material, *shader, translation * rotation * scaling);
    }
}

auto WorldBuilder::getFlashLightBodyPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {2.4f, 1.0f, 0.0f},
        {-2.5f, 1.5f, 2.5f}};
}

auto WorldBuilder::computeFlashLightBodyRotationMatrix(glm::vec3 const & position) const
    -> glm::mat4
{
    auto const translation = glm::translate(glm::mat4{1.0f}, position);

    auto const base = glm::normalize(glm::vec3{position.x, 0.0f, 0.0f});

    auto const direction = glm::normalize(position);

    return glm::orientation(direction, base);
}
    
auto WorldBuilder::getFlashLightMaterial() const
    -> Material
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const diffuseMapId = loadTexture("White.png");

    auto const specularMapId = loadTexture("White.png");

    auto const shininess = 32.0f;

    return {ambientColor, diffuseMapId, specularMapId, shininess};
}

auto WorldBuilder::createLighting() const
    -> Lighting
{
    return {createPointLights(), createSpotLights(), createDirectionalLights()};
}

auto WorldBuilder::createPointLights() const
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

auto WorldBuilder::createPointLight(
    glm::vec3 const & position,
    std::vector<PointLight> & lights) const
    -> void
{
    auto const ambient = glm::vec3{0.2f, 0.2f, 0.2f};

    auto const diffuse = glm::vec3{0.8f, 0.5f, 0.7f};

    auto const specular = glm::vec3{1.0f, 0.7f, 0.8f};

    auto const attenuation = Attenuation{1.0f, 0.09f, 0.032f};

    auto const color = Light::Color{ambient, diffuse, specular};

    lights.emplace_back(position, attenuation, color);
}

auto WorldBuilder::createSpotLights() const
    -> std::vector<SpotLight>
{
    auto lights = std::vector<SpotLight>{};

    const auto positions = getFlashLightBodyPositions();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        const auto position = positions[i];

        createSpotLight(position, -position, lights); 
    }

    return lights;
}

auto WorldBuilder::createSpotLight(
    glm::vec3 const & position,
    glm::vec3 const & direction,
    std::vector<SpotLight> & lights) const
    -> void
{
    auto const ambient = glm::vec3{0.0f, 0.0f, 0.0f};

    auto const diffuse = glm::vec3{0.8f, 0.8f, 0.2f};

    auto const specular = glm::vec3{0.5f, 0.5f, 0.1f};

    auto const color = Light::Color{ambient, diffuse, specular};

    auto const cutoff = SpotLight::CutoffAngle{glm::radians(15.0f), glm::radians(20.0f)};

    auto const attenuation = Attenuation{1.0f, 0.02f, 0.012f};

    lights.emplace_back(position, direction, cutoff, attenuation, color);
}

auto WorldBuilder::createDirectionalLights() const
    -> std::vector<DirectionalLight>
{
    auto lights = std::vector<DirectionalLight>{};

    const auto positions = getDirectionalLightDirections();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        const auto position = positions[i];

        createDirectionalLight(position, lights); 
    }

    return lights;
}

auto WorldBuilder::getDirectionalLightDirections() const
    -> std::vector<glm::vec3>
{
    return {
        //{0.0f, -1.0f, 0.0f}
        };
}

auto WorldBuilder::createDirectionalLight(
    glm::vec3 const & direction,
    std::vector<DirectionalLight> & lights) const
    -> void
{
    auto const ambient = glm::vec3{0.0f, 0.0f, 0.0f};

    auto const diffuse = glm::vec3{0.4f, 0.4f, 0.4f};

    auto const specular = glm::vec3{0.2f, 0.2f, 0.2f};

    auto const color = Light::Color{ambient, diffuse, specular};

    lights.emplace_back(direction, color);
}
    
auto WorldBuilder::createCamera() const
    -> Camera
{
    auto const position = glm::vec3{0.0f, 0.0f, 3.0f};

    auto const direction = glm::vec3{0.0f, 0.0f, -1.0f};

    auto const up = glm::vec3{0.0f, 1.0f, 0.0f};

    auto const fieldOfView = glm::radians(45.0f);

    auto const aspectRatio = getWindowRatio(*window);

    return {position, direction, up, fieldOfView, aspectRatio};
}

auto WorldBuilder::loadTexture(std::string filename) const
    -> Texture const &
{
    auto const texture = textures.findTexture(filename);

    if (texture != nullptr)
    {
        return *texture;
    }

    return textures.addTexture(Texture{filename});
}
