#include "SampleSceneBuilder.hpp"

#include "AssetBuilder.hpp"
#include "BoxBuilder.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace
{

class StatefulBuilder
{

public:

    explicit StatefulBuilder(AssetRepository & repository);

    auto build()
        -> SampleScene;

private:

    auto createBodies()
        -> void;

    auto createGroundTiles()
        -> void;

    auto createGroundTile(int row, int col, Model const & model)
        -> Body &;

    auto createContainers()
        -> void;

    auto createContainer(glm::mat4 const & transformation, Model const & model)
        -> Body &;

    auto getContainerPositions() const
        -> std::vector<glm::vec3>;

    auto createLamps()
        -> void;

    auto createLamp(glm::vec3 const & position, Model const & model)
        -> Body &;

    auto getLampPositions()
        -> std::vector<glm::vec3>;

    auto createFlashlights()
        -> void;

    auto createFlashlight(glm::mat4 const & transformation, Model const & model)
        -> Body &;

    auto getFlashlightPositions()
        -> std::vector<glm::vec3>;

    auto computeFlashlightRotation(glm::vec3 const & position)
        -> glm::mat4;

    auto createCameras()
        -> void;

    auto createLighting()
        -> void;

    auto createPointLights()
        -> void;

    auto createPointLight(glm::vec3 const & position)
        -> PointLight &;

    auto createSpotLights()
        -> void;

    auto getSpotLightColors()
        -> std::vector<Light::Color>;

    auto createSpotLight(
        glm::vec3 const & position,
        glm::vec3 const & direction,
        Light::Color const & color)
        -> SpotLight &;

    auto createDirectionalLights()
        -> void;

    auto getDirectionalLightDirections()
        -> std::vector<glm::vec3>;

    auto createDirectionalLight(glm::vec3 const & direction)
        -> DirectionalLight &;
    
private:

    AssetRepository * repository;

    SampleScene scene;

};

StatefulBuilder::StatefulBuilder(AssetRepository & repository)
    : repository{&repository}
{
}

auto StatefulBuilder::build()
    -> SampleScene
{
    createBodies();

    createCameras();

    createLighting();

    return std::move(scene);
}

auto StatefulBuilder::createBodies()
    -> void
{
    createGroundTiles();

    createContainers();

    createLamps();

    createFlashlights();
}

auto StatefulBuilder::createGroundTiles()
    -> void
{
    auto const & model = repository->getAssets().models[0];

    for (auto row = -5; row < +5; ++row)
    {
        for (auto col = -5; col < +5; ++col)
        {
            createGroundTile(row, col, model);
        }
    }
}

auto StatefulBuilder::createGroundTile(int const row, int const col, Model const & model)
    -> Body &
{
    auto const position = glm::vec3{row * 5.0f, -2.0f, col * 5.0f};

    auto instance = Body{model};

    setPosition(instance, position);

    return scene.bodies.emplace_back(std::move(instance));
}

auto StatefulBuilder::createContainers()
    -> void
{
    auto const & model = repository->getAssets().models[1];

    auto const positions = getContainerPositions();

    auto const numOfBodies = scene.bodies.size();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, positions[i]);

        auto const rotation = glm::rotate(
            glm::mat4{1.0f},
            glm::radians(20.0f * i),
            glm::vec3{1.0f, 0.3f, 0.5f});

        createContainer(translation * rotation, model);
    }

    scene.rotatingContainer = &scene.bodies[numOfBodies];

    scene.scalingContainer = &scene.bodies[numOfBodies + 1];
}

auto StatefulBuilder::createContainer(glm::mat4 const & transformation, Model const & model)
    -> Body &
{
    auto instance = Body{model};

    setTransformation(instance, transformation);

    return scene.bodies.emplace_back(std::move(instance));
}

auto StatefulBuilder::getContainerPositions() const
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

auto StatefulBuilder::createLamps()
    -> void
{
    auto const & model = repository->getAssets().models[2];

    auto const positions = getLampPositions();

    auto const numOfBodies = scene.bodies.size();

    for (auto const & position : positions)
    {
        auto & lamp = createLamp(position, model);

        scene.lamps.push_back(&lamp);
    }
}

auto StatefulBuilder::createLamp(glm::vec3 const & position, Model const & model)
    -> Body &
{
    auto instance = Body{model};

    setPosition(instance, position);

    return scene.bodies.emplace_back(std::move(instance));
}

auto StatefulBuilder::getLampPositions()
    -> std::vector<glm::vec3>
{
    return {
        {0.0f, 0.0f, 5.0f},
        {0.0f, 1.0f, -5.0f}};
}

auto StatefulBuilder::createFlashlights()
    -> void
{
    auto const & model = repository->getAssets().models[3];

    auto const positions = getFlashlightPositions();

    for (auto const & position : positions)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        auto const rotation = computeFlashlightRotation(position);

        createFlashlight(translation * rotation, model);
    }
}

auto StatefulBuilder::createFlashlight(glm::mat4 const & transformation, Model const & model)
    -> Body &
{
    auto instance = Body{model};

    setTransformation(instance, transformation);

    return scene.bodies.emplace_back(std::move(instance));
}

auto StatefulBuilder::getFlashlightPositions()
    -> std::vector<glm::vec3>
{
    return {
        {2.4f, 1.0f, 2.0f},
        {-2.5f, 1.5f, 2.5f}};
}

auto StatefulBuilder::computeFlashlightRotation(glm::vec3 const & position)
    -> glm::mat4
{
    auto const translation = glm::translate(glm::mat4{1.0f}, position);

    auto const base = glm::normalize(glm::vec3{position.x, 0.0f, 0.0f});

    auto const direction = glm::normalize(position);

    return glm::orientation(direction, base);
}

auto StatefulBuilder::createLighting()
    -> void
{
    createPointLights();
    
    createSpotLights();
    
    createDirectionalLights();
}

auto StatefulBuilder::createPointLights()
    -> void
{
    auto const positions = getLampPositions();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const position = positions[i];

        createPointLight(position); 
    }
}

auto StatefulBuilder::createPointLight(glm::vec3 const & position)
    -> PointLight &
{
    auto const ambient = glm::vec3{0.2f, 0.2f, 0.2f};

    auto const diffuse = glm::vec3{0.8f, 0.5f, 0.7f};

    auto const specular = glm::vec3{1.0f, 0.7f, 0.8f};

    auto const attenuation = Attenuation{1.0f, 0.09f, 0.032f};

    auto const color = Light::Color{ambient, diffuse, specular};

    return scene.lighting.point.emplace_back(position, attenuation, color, true);
}

auto StatefulBuilder::createSpotLights()
    -> void
{
    auto const positions = getFlashlightPositions();

    auto const colors = getSpotLightColors();

    assert(colors.size() == positions.size() + 1);

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const position = positions[i];

        createSpotLight(position, -position, colors[i]); 
    }

    auto & light = createSpotLight({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, colors.back()); 

    scene.playerFlashlight = &light;
}

auto StatefulBuilder::getSpotLightColors()
    -> std::vector<Light::Color>
{
    using Ambient = glm::vec3;

    using Diffuse = glm::vec3;
    
    using Specular = glm::vec3;

    return {
        {Ambient{0.0f, 0.0f, 0.0f}, Diffuse{0.8f, 0.8f, 0.2f}, Specular{0.5f, 0.5f, 0.1f}},
        {Ambient{0.0f, 0.0f, 0.0f}, Diffuse{0.2f, 0.3f, 0.9f}, Specular{0.5f, 0.7f, 0.8f}},
        {Ambient{0.0f, 0.0f, 0.0f}, Diffuse{0.9f, 0.8f, 0.6f}, Specular{1.0f, 0.9f, 0.7f}}};
}

auto StatefulBuilder::createSpotLight(
    glm::vec3 const & position,
    glm::vec3 const & direction,
    Light::Color const & color)
    -> SpotLight &
{
    auto const cutoff = SpotLight::CutoffAngle{glm::radians(10.0f), glm::radians(20.0f)};

    auto const attenuation = Attenuation{1.0f, 0.01f, 0.0052f};

    return scene.lighting.spot.emplace_back(position, direction, cutoff, attenuation, color, true);
}

auto StatefulBuilder::createDirectionalLights()
    -> void
{
    auto const positions = getDirectionalLightDirections();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const position = positions[i];

        createDirectionalLight(position); 
    }
}

auto StatefulBuilder::getDirectionalLightDirections()
    -> std::vector<glm::vec3>
{
    return {};
}

auto StatefulBuilder::createDirectionalLight(glm::vec3 const & direction)
    -> DirectionalLight &
{
    auto const ambient = glm::vec3{0.0f, 0.0f, 0.0f};

    auto const diffuse = glm::vec3{0.4f, 0.4f, 0.4f};

    auto const specular = glm::vec3{0.2f, 0.2f, 0.2f};

    auto const color = Light::Color{ambient, diffuse, specular};

    return scene.lighting.directional.emplace_back(direction, color, true);
}
 
auto StatefulBuilder::createCameras()
    -> void
{
    auto const position = glm::vec3{0.0f, 0.0f, 3.0f};

    auto const direction = glm::vec3{0.0f, 0.0f, -1.0f};

    auto const up = glm::vec3{0.0f, 1.0f, 0.0f};

    auto const fieldOfView = glm::radians(45.0f);

    auto const aspectRatio = 1.0f; // Will be corrected upon viewport setup

    scene.cameraSystem.cameras.emplace_back(position, direction, up, fieldOfView, aspectRatio);

    scene.cameraSystem.activeCamera = &scene.cameraSystem.cameras.front();
}

} // unnamed namespace

SampleSceneBuilder::SampleSceneBuilder(AssetRepository & repository)
    : repository{&repository}
{
}

auto SampleSceneBuilder::build() const
    -> SampleScene
{
    auto builder = StatefulBuilder{*repository};

    return builder.build();
}
