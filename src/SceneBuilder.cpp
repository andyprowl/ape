#include "SceneBuilder.hpp"

#include "BoxBuilder.hpp"
#include "Window.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

SceneBuilder::SceneBuilder(GLFWwindow & window, ShaderProgram const & shader)
    : window{&window}
    , shader{&shader}
{
}

auto SceneBuilder::build() const
    -> Scene
{
    auto bodies = createBodies();

    auto lighting = createLighting();

    auto camera = createCamera();

    return Scene{std::move(bodies), std::move(lighting), std::move(camera)};
}

auto SceneBuilder::createBodies() const
    -> std::vector<Mesh>
{
    auto bodies = std::vector<Mesh>{};
        
    createCubeBodies(bodies);

    createGroundTileBodies(bodies);

    createFlashLightBodies(bodies);

    createLampBodies(bodies);

    return bodies;
}

auto SceneBuilder::createGroundTileBodies(std::vector<Mesh> & bodies) const
    -> void
{
    auto const shape = makeBox(NormalDirection::outbound, {5.0f, 0.01f, 5.0f});

    auto const material = getGroundMaterial();

    for (auto row = -5; row < +5; ++row)
    {
        for (auto col = -5; col < +5; ++col)
        {
            createGroundTileMesh(row, col, shape, material, bodies);
        }
    }
}

auto SceneBuilder::createGroundTileMesh(
    int const row,
    int const col,
    std::shared_ptr<Shape> shape,
    Material const & material,
    std::vector<Mesh> & bodies) const
    -> void
{
    auto const position = glm::vec3{row * 5.0f, -2.0f, col * 5.0f};

    auto const translation = glm::translate(glm::mat4{1.0f}, position);

    bodies.emplace_back(std::move(shape), *shader, material, translation);
}

auto SceneBuilder::getGroundMaterial() const
    -> Material
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto diffuseMap = loadTexture("ConcreteGround.jpg");

    auto specularMap = loadTexture("ConcreteGround.jpg");

    auto const shininess = 16.0f;

    return {ambientColor, std::move(diffuseMap), std::move(specularMap), shininess};
}

auto SceneBuilder::createCubeBodies(std::vector<Mesh> & bodies) const
    -> void
{
    auto const shape = makeBox(NormalDirection::outbound, {1.0f, 1.0f, 1.0f});

    auto const positions = getCubeMeshPositions();

    auto const materials = getCubeMeshMaterials();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, positions[i]);

        auto const rotation = glm::rotate(
            glm::mat4{1.0f},
            glm::radians(20.0f * i),
            glm::vec3{1.0f, 0.3f, 0.5f});

        auto const & material = materials[i % materials.size()];

        bodies.emplace_back(shape, *shader, material, translation * rotation);
    }
}

auto SceneBuilder::getCubeMeshPositions() const
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

auto SceneBuilder::getCubeMeshMaterials() const
    -> std::vector<Material>
{
    return {getContainerMaterial()};
}

auto SceneBuilder::getContainerMaterial() const
    -> Material
{
    auto const ambientColor = glm::vec3{1.0f, 0.5f, 0.31f};

    auto const diffuseMapId = loadTexture("Container.Diffuse.png");

    auto const specularMapId = loadTexture("Container.Specular.png");

    auto const shininess = 32.0f;

    return {ambientColor, diffuseMapId, specularMapId, shininess};
}

auto SceneBuilder::createLampBodies(std::vector<Mesh> & bodies) const
    -> void
{
    auto const shape = makeBox(NormalDirection::inbound, {0.2f, 0.2f, 0.2f});

    auto const positions = getLampMeshPositions();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, positions[i]);

        auto const material = getLampMaterial();

        bodies.emplace_back(shape, *shader, material, translation);
    }
}

auto SceneBuilder::getLampMeshPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {0.0f, 0.0f, 5.0f},
        {0.0f, 1.0f, -5.0f}};
}

auto SceneBuilder::getLampMaterial() const
    -> Material
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const diffuseMapId = loadTexture("White.png");

    auto const specularMapId = loadTexture("White.png");

    auto const shininess = 32.0f;

    return {ambientColor, diffuseMapId, specularMapId, shininess};
}

auto SceneBuilder::createFlashLightBodies(std::vector<Mesh> & bodies) const
    -> void
{
    auto const shape = makeBox(NormalDirection::inbound, {0.3f, 0.1f, 0.1f});

    auto const positions = getFlashLightMeshPositions();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const position = positions[i];

        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        auto const rotation = computeFlashLightMeshRotationMatrix(position);

        auto const material = getFlashLightMaterial();

        bodies.emplace_back(shape, *shader, material, translation * rotation);
    }
}

auto SceneBuilder::getFlashLightMeshPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {2.4f, 1.0f, 0.0f},
        {-2.5f, 1.5f, 2.5f}};
}

auto SceneBuilder::computeFlashLightMeshRotationMatrix(glm::vec3 const & position) const
    -> glm::mat4
{
    auto const translation = glm::translate(glm::mat4{1.0f}, position);

    auto const base = glm::normalize(glm::vec3{position.x, 0.0f, 0.0f});

    auto const direction = glm::normalize(position);

    return glm::orientation(direction, base);
}
    
auto SceneBuilder::getFlashLightMaterial() const
    -> Material
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const diffuseMapId = loadTexture("White.png");

    auto const specularMapId = loadTexture("White.png");

    auto const shininess = 32.0f;

    return {ambientColor, diffuseMapId, specularMapId, shininess};
}

auto SceneBuilder::createLighting() const
    -> Lighting
{
    return {createPointLights(), createSpotLights(), createDirectionalLights()};
}

auto SceneBuilder::createPointLights() const
    -> std::vector<PointLight>
{
    auto lights = std::vector<PointLight>{};

    const auto positions = getLampMeshPositions();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        const auto position = positions[i];

        createPointLight(position, lights); 
    }

    return lights;
}

auto SceneBuilder::createPointLight(
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

auto SceneBuilder::createSpotLights() const
    -> std::vector<SpotLight>
{
    auto lights = std::vector<SpotLight>{};

    const auto positions = getFlashLightMeshPositions();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        const auto position = positions[i];

        createSpotLight(position, -position, lights); 
    }

    return lights;
}

auto SceneBuilder::createSpotLight(
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

auto SceneBuilder::createDirectionalLights() const
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

auto SceneBuilder::getDirectionalLightDirections() const
    -> std::vector<glm::vec3>
{
    return {};
}

auto SceneBuilder::createDirectionalLight(
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
    
auto SceneBuilder::createCamera() const
    -> Camera
{
    auto const position = glm::vec3{0.0f, 0.0f, 3.0f};

    auto const direction = glm::vec3{0.0f, 0.0f, -1.0f};

    auto const up = glm::vec3{0.0f, 1.0f, 0.0f};

    auto const fieldOfView = glm::radians(45.0f);

    auto const aspectRatio = getWindowRatio(*window);

    return {position, direction, up, fieldOfView, aspectRatio};
}

auto SceneBuilder::loadTexture(std::string filename) const
    -> Texture const &
{
    auto const texture = textures.findTexture(filename);

    if (texture != nullptr)
    {
        return *texture;
    }

    return textures.addTexture(Texture{filename});
}

auto SceneBuilder::makeBox(
    NormalDirection const normalDirection,
    glm::vec3 const & size) const
    -> std::shared_ptr<Shape>
{
    auto const builder = BoxBuilder{};

    auto square = builder.build(normalDirection, size);

    return std::make_shared<Shape>(std::move(square));
}
