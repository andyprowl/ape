#include "SampleSceneBuilder.hpp"

#include "SampleAssetBuilder.hpp"

#include <Ape/BoxBuilder.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace
{

class StatefulSceneBuilder
{

public:

    explicit StatefulSceneBuilder(SampleAssetCollection & assets);

    auto build()
        -> SampleScene;

private:

    auto createBodies()
        -> void;

    auto createGroundTiles()
        -> void;

    auto createGroundTile(int row, int col, ape::Model const & model)
        -> ape::Body &;

    auto createContainers()
        -> void;

    auto getContainerPositions() const
        -> std::vector<glm::vec3>;

    auto createLamps()
        -> void;

    auto getLampPositions() const
        -> std::vector<glm::vec3>;

    auto createFlashlights()
        -> void;

    auto createFlashlight(glm::mat4 const & transformation, ape::Model const & model)
        -> ape::Body &;

    auto getFlashlightPositions() const
        -> std::vector<glm::vec3>;

    auto computeFlashlightRotation(glm::vec3 const & position)
        -> glm::mat4;

    auto createNanosuits()
        -> void;

    auto getNanosuitPositions() const
        -> std::vector<glm::vec3>;

    auto createDragons()
        -> void;

    auto getDragonPositions() const
        -> std::vector<glm::vec3>;

    auto createSpaceships()
        -> void;

    auto getSpaceshipPositions() const
        -> std::vector<glm::vec3>;

    auto createCameras()
        -> void;

    auto createFrontCamera()
        -> ape::Camera &;

    auto createBackCamera()
        -> ape::Camera &;

    auto createRightCamera()
        -> ape::Camera &;

    auto createLighting()
        -> void;

    auto createPointLights()
        -> void;

    auto createPointLight(glm::vec3 const & position)
        -> ape::PointLight &;

    auto createSpotLights()
        -> void;

    auto getSpotLightColors()
        -> std::vector<ape::Light::Color>;

    auto createSpotLight(
        glm::vec3 const & position,
        glm::vec3 const & direction,
        ape::Light::Color const & color)
        -> ape::SpotLight &;

    auto createDirectionalLights()
        -> void;

    auto getDirectionalLightDirections()
        -> std::vector<glm::vec3>;

    auto createDirectionalLight(glm::vec3 const & direction)
        -> ape::DirectionalLight &;
    
    auto addBody(glm::mat4 const & transformation, ape::Model const & model)
        -> ape::Body &;

    auto createSynchronizers()
        -> void;
    
private:

    SampleAssetCollection * assets;

    SampleScene scene;

};

StatefulSceneBuilder::StatefulSceneBuilder(SampleAssetCollection & assets)
    : assets{&assets}
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

    createNanosuits();

    createDragons();

    createSpaceships();
}

auto StatefulSceneBuilder::createGroundTiles()
    -> void
{
    auto & concrete = assets->generalAssets.models[0];

    auto & wood = assets->generalAssets.models[1];

    for (auto row = -5; row < +5; ++row)
    {
        for (auto col = -5; col < +5; ++col)
        {
            auto const isInnerSquare = (std::abs(row) < 1) && (std::abs(col) < 1);

            auto const & model = isInnerSquare ? wood : concrete;

            createGroundTile(row, col, model);
        }
    }
}

auto StatefulSceneBuilder::createGroundTile(int const row, int const col, ape::Model const & model)
    -> ape::Body &
{
    auto const position = glm::vec3{row * 5.0f, -2.0f, col * 5.0f};

    auto body = ape::Body{model};

    ape::setPosition(body, position);

    return scene.bodies.emplace_back(std::move(body));
}

auto StatefulSceneBuilder::createContainers()
    -> void
{
    auto const & model = assets->generalAssets.models[2];

    auto const positions = getContainerPositions();

    auto const numOfBodies = scene.bodies.size();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, positions[i]);

        auto const rotation = glm::rotate(
            glm::mat4{1.0f},
            glm::radians(20.0f * i),
            glm::vec3{1.0f, 0.3f, 0.5f});

        addBody(translation * rotation, model);
    }

    scene.rotatingContainer = &scene.bodies[numOfBodies];

    scene.scalingContainer = &scene.bodies[numOfBodies + 1];
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
    auto const & model = assets->generalAssets.models[3];

    auto const positions = getLampPositions();

    auto const numOfBodies = scene.bodies.size();

    for (auto const & position : positions)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        auto & lamp = addBody(translation, model);

        scene.lamps.push_back(&lamp);
    }
}

auto StatefulSceneBuilder::getLampPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {0.0f, 0.0f, 5.0f},
        {0.0f, 1.0f, -5.0f},
        {0.0f, 10.0f, 5.0f},
        {0.0f, 10.0f, -5.0f}};
}

auto StatefulSceneBuilder::createFlashlights()
    -> void
{
    auto const & model = assets->generalAssets.models[4];

    auto const positions = getFlashlightPositions();

    for (auto const & position : positions)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        auto const rotation = computeFlashlightRotation(position);

        createFlashlight(translation * rotation, model);
    }
}

auto StatefulSceneBuilder::createFlashlight(
    glm::mat4 const & transformation,
    ape::Model const & model)
    -> ape::Body &
{
    auto body = ape::Body{model};

    ape::setTransformation(body, transformation);

    return scene.bodies.emplace_back(std::move(body));
}

auto StatefulSceneBuilder::getFlashlightPositions() const
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

auto StatefulSceneBuilder::createNanosuits()
    -> void
{
    auto const & model = assets->nanosuitAssets.models[0];

    auto const positions = getNanosuitPositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {0.2f, 0.2f, 0.2f});

    for (auto const & position : positions)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        addBody(translation * scaling, model);
    }
}

auto StatefulSceneBuilder::getNanosuitPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {1.0f, -2.0f, 1.0f}};
}

auto StatefulSceneBuilder::createDragons()
    -> void
{
    auto const & model = assets->dragonAssets.models[0];

    auto const positions = getDragonPositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {0.1f, 0.1f, 0.1f});

    for (auto const & position : positions)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        addBody(translation * scaling, model);
    }
}

auto StatefulSceneBuilder::getDragonPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {-7.0f, -2.0f, -2.0f}};
}

auto StatefulSceneBuilder::createSpaceships()
    -> void
{
    auto const & model = assets->spaceshipAssets.models[0];

    auto const positions = getSpaceshipPositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {1.0f, 1.0f, 1.0f});

    for (auto const & position : positions)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        addBody(translation * scaling, model);
    }
}

auto StatefulSceneBuilder::getSpaceshipPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {-0.0f, 6.0f, -5.0f}};
}

auto StatefulSceneBuilder::createCameras()
    -> void
{
    createFrontCamera();

    createBackCamera();

    createRightCamera();

    scene.cameraSystem.activeCamera = &scene.cameraSystem.cameras.front();
}

auto StatefulSceneBuilder::createFrontCamera()
     -> ape::Camera &
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
     -> ape::Camera &
{
    auto const position = glm::vec3{0.0f, 0.0f, -3.0f};

    auto const direction = glm::vec3{0.0f, 0.0f, 1.0f};

    auto const up = glm::vec3{0.0f, 1.0f, 0.0f};

    auto const fieldOfView = glm::radians(45.0f);

    auto const aspectRatio = 1.0f; // Will be corrected upon viewport setup

    auto & cameras = scene.cameraSystem.cameras;

    return cameras.emplace_back(position, direction, up, fieldOfView, aspectRatio);
}

auto StatefulSceneBuilder::createRightCamera()
     -> ape::Camera &
{
    auto const position = glm::vec3{5.0f, 0.0f, 0.0f};

    auto const direction = glm::vec3{-1.0f, 0.0f, 0.0f};

    auto const up = glm::vec3{0.0f, 1.0f, 0.0f};

    auto const fieldOfView = glm::radians(45.0f);

    auto const aspectRatio = 1.0f; // Will be corrected upon viewport setup

    auto & cameras = scene.cameraSystem.cameras;

    return cameras.emplace_back(position, direction, up, fieldOfView, aspectRatio);
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
    -> ape::PointLight &
{
    auto const ambient = glm::vec3{0.2f, 0.2f, 0.2f};

    auto const diffuse = glm::vec3{0.8f, 0.5f, 0.7f};

    auto const specular = glm::vec3{1.0f, 0.7f, 0.8f};

    auto const attenuation = ape::Attenuation{1.0f, 0.09f, 0.032f};

    auto const color = ape::Light::Color{ambient, diffuse, specular};

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
    -> std::vector<ape::Light::Color>
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
    ape::Light::Color const & color)
    -> ape::SpotLight &
{
    auto const cutoff = ape::SpotLight::CutoffAngle{glm::radians(10.0f), glm::radians(20.0f)};

    auto const attenuation = ape::Attenuation{1.0f, 0.01f, 0.0052f};

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
    -> ape::DirectionalLight &
{
    auto const ambient = glm::vec3{0.0f, 0.0f, 0.0f};

    auto const diffuse = glm::vec3{0.4f, 0.4f, 0.4f};

    auto const specular = glm::vec3{0.2f, 0.2f, 0.2f};

    auto const color = ape::Light::Color{ambient, diffuse, specular};

    return scene.lighting.directional.emplace_back(direction, color, true);
}

auto StatefulSceneBuilder::addBody(glm::mat4 const & transformation, ape::Model const & model)
    -> ape::Body &
{
    auto body = ape::Body{model};

    ape::setTransformation(body, transformation);

    return scene.bodies.emplace_back(std::move(body));
}

auto StatefulSceneBuilder::createSynchronizers()
    -> void
{
    scene.spotLightSynchronizers.emplace_back(
        *scene.cameraSystem.activeCamera,
        *scene.playerFlashlight);

    scene.bodyLightSynchronizers.emplace_back(
        *scene.lamps.front(),
        scene.lighting.point.front());
}

} // unnamed namespace

SampleSceneBuilder::SampleSceneBuilder(SampleAssetCollection & assets)
    : assets{&assets}
{
}

auto SampleSceneBuilder::build() const
    -> SampleScene
{
    auto builder = StatefulSceneBuilder{*assets};

    return builder.build();
}
