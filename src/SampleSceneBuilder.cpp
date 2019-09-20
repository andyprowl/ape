#include "SampleSceneBuilder.hpp"

#include "SampleAssetBuilder.hpp"
#include "BoxBuilder.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace
{

class StatefulSceneBuilder
{

public:

    explicit StatefulSceneBuilder(AssetRepository & repository);

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

    auto createFrontCamera()
        -> Camera &;

    auto createBackCamera()
        -> Camera &;

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

    auto createSynchronizers()
        -> void;
    
private:

    AssetRepository * repository;

    SampleScene scene;

};

StatefulSceneBuilder::StatefulSceneBuilder(AssetRepository & repository)
    : repository{&repository}
{
}

auto StatefulSceneBuilder::build()
    -> SampleScene
{
    createBodies();

    createCameras();

    createLighting();

    createSynchronizers();

    return std::move(scene);
}

auto StatefulSceneBuilder::createBodies()
    -> void
{
    createGroundTiles();

    createContainers();

    createLamps();

    createFlashlights();
}

auto StatefulSceneBuilder::createGroundTiles()
    -> void
{
    auto const & model = repository->models[0];

    for (auto row = -5; row < +5; ++row)
    {
        for (auto col = -5; col < +5; ++col)
        {
            createGroundTile(row, col, model);
        }
    }
}

auto StatefulSceneBuilder::createGroundTile(int const row, int const col, Model const & model)
    -> Body &
{
    auto const position = glm::vec3{row * 5.0f, -2.0f, col * 5.0f};

    auto instance = Body{model};

    setPosition(instance, position);

    return scene.bodies.emplace_back(std::move(instance));
}

auto StatefulSceneBuilder::createContainers()
    -> void
{
    auto const & model = repository->models[1];

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

auto StatefulSceneBuilder::createContainer(glm::mat4 const & transformation, Model const & model)
    -> Body &
{
    auto instance = Body{model};

    setTransformation(instance, transformation);

    return scene.bodies.emplace_back(std::move(instance));
}

auto StatefulSceneBuilder::getContainerPositions() const
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

auto StatefulSceneBuilder::createLamps()
    -> void
{
    auto const & model = repository->models[2];

    auto const positions = getLampPositions();

    auto const numOfBodies = scene.bodies.size();

    for (auto const & position : positions)
    {
        auto & lamp = createLamp(position, model);

        scene.lamps.push_back(&lamp);
    }
}

auto StatefulSceneBuilder::createLamp(glm::vec3 const & position, Model const & model)
    -> Body &
{
    auto instance = Body{model};

    setPosition(instance, position);

    return scene.bodies.emplace_back(std::move(instance));
}

auto StatefulSceneBuilder::getLampPositions()
    -> std::vector<glm::vec3>
{
    return {
        {0.0f, 0.0f, 5.0f},
        {0.0f, 1.0f, -5.0f}};
}

auto StatefulSceneBuilder::createFlashlights()
    -> void
{
    auto const & model = repository->models[3];

    auto const positions = getFlashlightPositions();

    for (auto const & position : positions)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        auto const rotation = computeFlashlightRotation(position);

        createFlashlight(translation * rotation, model);
    }
}

auto StatefulSceneBuilder::createFlashlight(glm::mat4 const & transformation, Model const & model)
    -> Body &
{
    auto instance = Body{model};

    setTransformation(instance, transformation);

    return scene.bodies.emplace_back(std::move(instance));
}

auto StatefulSceneBuilder::getFlashlightPositions()
    -> std::vector<glm::vec3>
{
    return {
        {2.4f, 1.0f, 2.0f},
        {-2.5f, 1.5f, 2.5f}};
}

auto StatefulSceneBuilder::computeFlashlightRotation(glm::vec3 const & position)
    -> glm::mat4
{
    auto const translation = glm::translate(glm::mat4{1.0f}, position);

    auto const base = glm::normalize(glm::vec3{position.x, 0.0f, 0.0f});

    auto const direction = glm::normalize(position);

    return glm::orientation(direction, base);
}

auto StatefulSceneBuilder::createLighting()
    -> void
{
    createPointLights();
    
    createSpotLights();
    
    createDirectionalLights();
}

auto StatefulSceneBuilder::createPointLights()
    -> void
{
    auto const positions = getLampPositions();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const position = positions[i];

        createPointLight(position); 
    }
}

auto StatefulSceneBuilder::createPointLight(glm::vec3 const & position)
    -> PointLight &
{
    auto const ambient = glm::vec3{0.2f, 0.2f, 0.2f};

    auto const diffuse = glm::vec3{0.8f, 0.5f, 0.7f};

    auto const specular = glm::vec3{1.0f, 0.7f, 0.8f};

    auto const attenuation = Attenuation{1.0f, 0.09f, 0.032f};

    auto const color = Light::Color{ambient, diffuse, specular};

    return scene.lighting.point.emplace_back(position, attenuation, color, true);
}

auto StatefulSceneBuilder::createSpotLights()
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

auto StatefulSceneBuilder::getSpotLightColors()
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

auto StatefulSceneBuilder::createSpotLight(
    glm::vec3 const & position,
    glm::vec3 const & direction,
    Light::Color const & color)
    -> SpotLight &
{
    auto const cutoff = SpotLight::CutoffAngle{glm::radians(10.0f), glm::radians(20.0f)};

    auto const attenuation = Attenuation{1.0f, 0.01f, 0.0052f};

    return scene.lighting.spot.emplace_back(position, direction, cutoff, attenuation, color, true);
}

auto StatefulSceneBuilder::createDirectionalLights()
    -> void
{
    auto const positions = getDirectionalLightDirections();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const position = positions[i];

        createDirectionalLight(position); 
    }
}

auto StatefulSceneBuilder::getDirectionalLightDirections()
    -> std::vector<glm::vec3>
{
    return {};
}

auto StatefulSceneBuilder::createDirectionalLight(glm::vec3 const & direction)
    -> DirectionalLight &
{
    auto const ambient = glm::vec3{0.0f, 0.0f, 0.0f};

    auto const diffuse = glm::vec3{0.4f, 0.4f, 0.4f};

    auto const specular = glm::vec3{0.2f, 0.2f, 0.2f};

    auto const color = Light::Color{ambient, diffuse, specular};

    return scene.lighting.directional.emplace_back(direction, color, true);
}
 
auto StatefulSceneBuilder::createCameras()
    -> void
{
    createFrontCamera();

    createBackCamera();

    scene.cameraSystem.activeCamera = &scene.cameraSystem.cameras.front();
}

auto StatefulSceneBuilder::createFrontCamera()
     -> Camera &
{
    auto const position = glm::vec3{0.0f, 0.0f, 3.0f};

    auto const direction = glm::vec3{0.0f, 0.0f, -1.0f};

    auto const up = glm::vec3{0.0f, 1.0f, 0.0f};

    auto const fieldOfView = glm::radians(45.0f);

    auto const aspectRatio = 1.0f; // Will be corrected upon viewport setup

    auto & cameras = scene.cameraSystem.cameras;

    return cameras.emplace_back(position, direction, up, fieldOfView, aspectRatio);
}

auto StatefulSceneBuilder::createBackCamera()
     -> Camera &
{
    auto const position = glm::vec3{0.0f, 0.0f, -3.0f};

    auto const direction = glm::vec3{0.0f, 0.0f, 1.0f};

    auto const up = glm::vec3{0.0f, 1.0f, 0.0f};

    auto const fieldOfView = glm::radians(45.0f);

    auto const aspectRatio = 1.0f; // Will be corrected upon viewport setup

    auto & cameras = scene.cameraSystem.cameras;

    return cameras.emplace_back(position, direction, up, fieldOfView, aspectRatio);
}

auto StatefulSceneBuilder::createSynchronizers()
    -> void
{
    scene.spotLightSynchronizers.emplace_back(
        *scene.cameraSystem.activeCamera,
        *scene.playerFlashlight);
}

} // unnamed namespace

SampleSceneBuilder::SampleSceneBuilder(AssetRepository & repository)
    : repository{&repository}
{
}

auto SampleSceneBuilder::build() const
    -> SampleScene
{
    auto builder = StatefulSceneBuilder{*repository};

    return builder.build();
}
