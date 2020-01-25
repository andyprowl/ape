#include <Rave/RaveCore/RaveSceneBuilder.hpp>

#include <Rave/RaveCore/RaveAssetBuilder.hpp>

#include <Ape/World/Shape/BoxShapeBuilder.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <Basix/Range/Transform.hpp>

namespace rave
{

namespace
{

class StatefulSceneBuilder
{

public:

    StatefulSceneBuilder(RaveAssetCollection & assets, bool doNotIncludeSponza);

    auto build()
        -> RaveScene;

private:

    auto createBodies()
        -> void;

    auto createFloor()
        -> void;

    auto createTiledPlatform(float z)
        -> void;

    auto createTile(int row, int col, float z, ape::Model const & model)
        -> ape::Body &;

    auto createWalls()
        -> void;

    auto createWall(
        int row,
        int col,
        glm::vec2 const & offset,
        glm::mat4 const & rotation,
        ape::Model const & model)
        -> ape::Body &;

    auto createContainers()
        -> void;
    
    auto createRandomRotatedContainers()
        -> void;

    auto getRandomRotatedContainerPositions() const
        -> std::vector<glm::vec3>;

    auto createCustomTransformedContainers()
        -> void;

    auto getCustomContainerTransformations() const
        -> std::vector<glm::mat4>;

    auto createLamps()
        -> void;

    auto getPointLampPositions() const
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

    auto createDynos()
        -> void;

    auto getDynoPositions() const
        -> std::vector<glm::vec3>;
    
    auto createCastles()
        -> void;

    auto getCastlePositions() const
        -> std::vector<glm::vec3>;
    
    auto createTaverns()
        -> void;

    auto getTavernPositions() const
        -> std::vector<glm::vec3>;
    
    auto createHouses()
        -> void;

    auto getHousePositions() const
        -> std::vector<glm::vec3>;
    
    auto createCottages()
        -> void;

    auto getCottagePositions() const
        -> std::vector<glm::vec3>;
    
    auto createBridges()
        -> void;

    auto getBridgePositions() const
        -> std::vector<glm::vec3>;
    
    auto createLibertyStatues()
        -> void;

    auto getLibertyStatuePositions() const
        -> std::vector<glm::vec3>;

    auto createSponzas()
        -> void;

    auto getSponzaPositions() const
        -> std::vector<glm::vec3>;

    auto createCameras()
        -> void;

    auto createFrontCamera()
        -> ape::Camera &;

    auto createBackCamera()
        -> ape::Camera &;

    auto createRightCamera()
        -> ape::Camera &;

    auto createLightSystem()
        -> void;

    auto createPointLights()
        -> void;

    auto getPointLightColors()
        -> std::vector<ape::Light::Color>;

    auto createPointLight(
        glm::vec3 const & position,
        ape::Light::Color const & color,
        std::string name)
        -> ape::PointLight &;

    auto createSpotLights()
        -> void;

    auto getSpotLightColors()
        -> std::vector<ape::Light::Color>;

    auto createSpotLight(
        glm::vec3 const & position,
        glm::vec3 const & direction,
        ape::Light::Color const & color,
        std::string name)
        -> ape::SpotLight &;

    auto createDirectionalLights()
        -> void;

    auto getDirectionalLightDirections()
        -> std::vector<glm::vec3>;

    auto createDirectionalLight(glm::vec3 const & direction, std::string name)
        -> ape::DirectionalLight &;
    
    auto addBody(glm::mat4 const & transformation, ape::Model const & model)
        -> ape::Body &;

    auto createSynchronizers()
        -> void;
    
private:

    RaveAssetCollection * assets;

    bool doNotIncludeSponza;

    RaveScene scene;

};

StatefulSceneBuilder::StatefulSceneBuilder(
    RaveAssetCollection & assets,
    bool const doNotIncludeSponza)
    : assets{&assets}
    , doNotIncludeSponza{doNotIncludeSponza}
{
}

auto StatefulSceneBuilder::build()
    -> RaveScene
{
    createBodies();

    createCameras();

    createLightSystem();

    createSynchronizers();

    return std::move(scene);
}

auto StatefulSceneBuilder::createBodies()
    -> void
{
    scene.reserveBodyCapacity(500);

    createFloor();

    createWalls();

    createContainers();

    createLamps();

    createFlashlights();
 
    createNanosuits();

    createDragons();

    createSpaceships();

    createDynos();

    createCastles();
    
    createTaverns();
    
    createHouses();
    
    createCottages();
    
    createBridges();
    
    createLibertyStatues();
    
    createSponzas();
}

auto StatefulSceneBuilder::createFloor()
    -> void
{
    createTiledPlatform(-2.0f);
}

auto StatefulSceneBuilder::createTiledPlatform(const float z)
    -> void
{
    auto & concrete = assets->generalAssets.models[0];

    auto & wood = assets->generalAssets.models[1];

    for (auto row = -7; row < +7; ++row)
    {
        for (auto col = -7; col < +7; ++col)
        {
            auto const isInnerSquare = (std::abs(row) < 1) && (std::abs(col) < 1);

            auto const & model = isInnerSquare ? wood : concrete;

            createTile(row, col, z, model);
        }
    }
}

auto StatefulSceneBuilder::createTile(
    int const row,
    int const col,
    float const z,
    ape::Model const & model)
    -> ape::Body &
{
    auto const position = glm::vec3{row * 5.0f, z, col * 5.0f};

    auto const castsShadow = false;

    auto body = ape::Body{model, "", castsShadow};

    ape::setPosition(body, position);

    return scene.addBody(std::move(body));
}

auto StatefulSceneBuilder::createWalls()
    -> void
{
    auto & concrete = assets->generalAssets.models[0];

    auto const rotationAroundX = glm::rotate(
        glm::mat4{1.0f},
        glm::radians(-90.0f),
        glm::vec3{1.0f, 0.0f, 0.0f});

    auto const rotationAroundZ = glm::rotate(
        glm::mat4{1.0f},
        glm::radians(-90.0f),
        glm::vec3{0.0f, 0.0f, 1.0f});

    for (auto i = -7; i < +7; ++i)
    {
        createWall(i, -7, {0.0f, -2.5f}, rotationAroundX, concrete);

        createWall(i, +6, {0.0f, +2.5f}, rotationAroundX, concrete);

        createWall(-7, i, {-2.5f, 0.0f}, rotationAroundZ, concrete);

        createWall(+6, i, {+2.5f, 0.0f}, rotationAroundZ, concrete);
    }
}

auto StatefulSceneBuilder::createWall(
    int const row,
    int const col,
    glm::vec2 const & offset,
    glm::mat4 const & rotation,
    ape::Model const & model)
    -> ape::Body &
{
    auto const position = glm::vec3{row * 5.0f + offset.x, 0.5, col * 5.0f + offset.y};

    auto const translation = glm::translate(glm::mat4{1.0f}, position);

    return addBody(translation * rotation, model);
}

auto StatefulSceneBuilder::createContainers()
    -> void
{
    createRandomRotatedContainers();

    createCustomTransformedContainers();
}

auto StatefulSceneBuilder::createRandomRotatedContainers()
    -> void
{
    auto const & model = assets->generalAssets.models[2];

    auto const positions = getRandomRotatedContainerPositions();

    auto const numOfBodies = scene.getNumOfBodies();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, positions[i]);

        auto const rotation = glm::rotate(
            glm::mat4{1.0f},
            glm::radians(20.0f * i),
            glm::vec3{1.0f, 0.3f, 0.5f});

        addBody(translation * rotation, model);
    }

    if (positions.size() >= 2u)
    {
        scene.rotatingContainer = &scene.getBody(numOfBodies);

        scene.scalingContainer = &scene.getBody(numOfBodies + 1);
    }
}

auto StatefulSceneBuilder::getRandomRotatedContainerPositions() const
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

auto StatefulSceneBuilder::createCustomTransformedContainers()
    -> void
{
    auto const & model = assets->generalAssets.models[2];

    auto const transformations = getCustomContainerTransformations();

    auto const numOfBodies = scene.getNumOfBodies();

    for (auto i = 0; i < static_cast<int>(transformations.size()); ++i)
    {
        auto const & transformation = transformations[i];

        addBody(transformation, model);
    }
}

auto StatefulSceneBuilder::getCustomContainerTransformations() const
    -> std::vector<glm::mat4>
{
    class Transformation
    {
    
    public:

        glm::vec3 position;

        float rotationAroundY;
    
    };

    auto const transformations = std::vector<Transformation>{
        {{0.0f, -1.5f, 7.5f}, 0.0f},
        {{2.0f, -1.5f, 7.5f}, 0.0f},
        {{-15.0f, -1.5f, -16.0f}, 10.0f},
        {{-16.5f, -1.5f, -16.1f}, -15.0f},
        {{-19.0f, -1.5f, -16.2f}, 20.0f},
        {{-20.2f, -1.5f, -16.2f}, 7.0f},
        {{-21.3f, -1.5f, -16.6f}, -4.0f}};

    return basix::transform(transformations, [] (Transformation const & transformation)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, transformation.position);

        auto const rotation = glm::rotate(
            glm::mat4{1.0f},
            glm::radians(transformation.rotationAroundY),
            glm::vec3{0.0f, 0.1f, 0.0f});

        return (translation * rotation);
    });
}

auto StatefulSceneBuilder::createLamps()
    -> void
{
    auto const & model = assets->generalAssets.models[3];

    auto const positions = getPointLampPositions();

    for (auto const & position : positions)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        auto & lamp = addBody(translation, model);

        scene.lamps.push_back(&lamp);
    }
}

auto StatefulSceneBuilder::getPointLampPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {0.0f, 0.0f, 5.0f},
        {0.0f, 1.0f, -6.0f},
        {0.0f, 10.0f, 5.0f},
        {0.0f, 10.0f, -5.0f},
        {0.0f, 3.0f, -40.0f},
        {0.0f, 0.0f, -35.0f},
        {0.0f, 3.0f, -30.0f},
        {-5.0f, 3.0f, -15.0f},
        {-15.0f, 2.0f, -10.0f},
        {-25.0f, 0.0f, -12.0f},
        {8.0f, 3.0f, -15.0f},
        {15.0f, 0.0f, -10.0f}};
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

        auto & flashlight = createFlashlight(translation * rotation, model);

        scene.flashlights.push_back(&flashlight);
    }
}

auto StatefulSceneBuilder::createFlashlight(
    glm::mat4 const & transformation,
    ape::Model const & model)
    -> ape::Body &
{
    auto body = ape::Body{model};

    ape::setTransformation(body, transformation);

    return scene.addBody(std::move(body));
}

auto StatefulSceneBuilder::getFlashlightPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {2.5f, 1.0f, 3.0f},
        {-2.5f, 1.5f, 2.5f},
        {-10.0f, 15.0f, -35.0f},
        {10.0f, 15.0f, -35.0f},
        {0.5f, 3.5f, 0.0f}};
}

auto StatefulSceneBuilder::computeFlashlightRotation(glm::vec3 const & position)
    -> glm::mat4
{
    auto const base = glm::normalize(glm::vec3{1.0f, 0.0f, 0.0f});

    auto const direction = glm::normalize(position);

    return glm::orientation(direction, base);
}

auto StatefulSceneBuilder::createNanosuits()
    -> void
{
    auto const & model = assets->nanosuitAssets.models[0];

    auto const positions = getNanosuitPositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {0.2f, 0.2f, 0.2f});

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, positions[i]);

        auto const rotation = glm::rotate(
            glm::mat4{1.0f},
            glm::radians(30.0f * (i - 1)),
            glm::vec3{0.0f, 0.1f, 0.0f});

        scene.nanosuit = &addBody(translation * rotation * scaling, model);
    }
}

auto StatefulSceneBuilder::getNanosuitPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {-3.0f, -2.0f, -2.0f},
        {1.0f, -2.0f, 1.0f},
        {3.0f, -2.0f, 0.0f}};
}

auto StatefulSceneBuilder::createDragons()
    -> void
{
    auto const & model = assets->dragonAssets.models[0];

    auto const positions = getDragonPositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {0.1f, 0.1f, 0.1f});
    
    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, positions[i]);

        auto const rotation = glm::rotate(
            glm::mat4{1.0f},
            glm::radians(45.0f * i),
            glm::vec3{0.0f, 0.1f, 0.0f});

        scene.dragon = &addBody(translation * rotation * scaling, model);
    }
}

auto StatefulSceneBuilder::getDragonPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {-3.9f, -2.0f, 7.5f},
        {-8.0f, -2.0f, -7.0f},
        {-7.0f, -2.0f, -17.0f}};
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

        scene.spaceship = &addBody(translation * scaling, model);
    }
}

auto StatefulSceneBuilder::getSpaceshipPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {-5.0f, 6.0f, -2.0f}};
}

auto StatefulSceneBuilder::createDynos()
    -> void
{
    auto const & model = assets->dynoAssets.models[0];

    auto const positions = getDynoPositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {1.0f, 1.0f, 1.0f});

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const & position = positions[i];

        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        auto const rotation = glm::rotate(
            glm::mat4{1.0f},
            glm::radians(i * 45.0f),
            {0.0f, 1.0, 0.0f});

        addBody(translation * scaling * rotation, model);
    }
}

auto StatefulSceneBuilder::getDynoPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {5.0f, -2.0f, 6.0f},
        {7.0f, -2.0f, -5.0f}};
}

auto StatefulSceneBuilder::createCastles()
    -> void
{
    auto const & model = assets->castleAssets.models[0];

    auto const positions = getCastlePositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {4.0f, 4.0f, 4.0f});

    for (auto const & position : positions)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        scene.castle = &addBody(translation * scaling, model);
    }
}

auto StatefulSceneBuilder::getCastlePositions() const
    -> std::vector<glm::vec3>
{
    return {
        {-20.0f, -2.0f, 10.0f},
        {20.0f, -2.0f, 10.0f}};
}

auto StatefulSceneBuilder::createTaverns()
    -> void
{
    auto const & model = assets->tavernAssets.models[0];

    auto const positions = getTavernPositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {0.1f, 0.1f, 0.1f});

    for (auto const & position : positions)
    {
        auto const rotationX = glm::rotate(
            glm::mat4{1.0f},
            glm::radians(-90.f),
            glm::vec3{1.0f, 0.0f, 0.0f});

        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        addBody(translation * scaling * rotationX, model);
    }
}

auto StatefulSceneBuilder::getTavernPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {0.0f, 3.9f, -15.0f}};
}

auto StatefulSceneBuilder::createHouses()
    -> void
{
    auto const & model = assets->houseAssets.models[0];

    auto const positions = getHousePositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {0.01f, 0.01f, 0.01f});

    for (auto const & position : positions)
    {
        auto const rotationY = glm::rotate(
            glm::mat4{1.0f},
            glm::radians(180.f),
            glm::vec3{0.0f, 1.0f, 0.0f});

        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        addBody(translation * scaling * rotationY, model);
    }
}

auto StatefulSceneBuilder::getHousePositions() const
    -> std::vector<glm::vec3>
{
    return {
        {0.0f, -2.0f, 15.0f},
        {-20.0f, -2.0f, -20.0f}};
}

auto StatefulSceneBuilder::createCottages()
    -> void
{
    auto const & model = assets->cottageAssets.models[0];

    auto const positions = getCottagePositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {1.0f, 1.0f, 1.0f});

    for (auto const & position : positions)
    {
        auto const rotationY = glm::rotate(
            glm::mat4{1.0f},
            glm::radians(180.f),
            glm::vec3{0.0f, 1.0f, 0.0f});

        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        addBody(translation * scaling * rotationY, model);
    }
}

auto StatefulSceneBuilder::getCottagePositions() const
    -> std::vector<glm::vec3>
{
    return {
        {20.0f, 0.0f, -20.0f}};
}

auto StatefulSceneBuilder::createBridges()
    -> void
{
    auto const & model = assets->bridgeAssets.models[0];

    auto const positions = getBridgePositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {0.08f, 0.08f, 0.08f});

    for (auto const & position : positions)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        addBody(translation * scaling, model);
    }
}

auto StatefulSceneBuilder::getBridgePositions() const
    -> std::vector<glm::vec3>
{
    return {
        {8.0f, -2.1f, -6.0f}};
}

auto StatefulSceneBuilder::createLibertyStatues()
    -> void
{
    auto const & model = assets->libertyStatueAssets.models[0];

    auto const positions = getLibertyStatuePositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {3.0f, 3.0f, 3.0f});

    for (auto const & position : positions)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        addBody(translation * scaling, model);
    }
}

auto StatefulSceneBuilder::getLibertyStatuePositions() const
    -> std::vector<glm::vec3>
{
    return {
        {-16.0f, -2.5f, -14.0f},
        {-14.0f, -2.5f, -7.0f},
        {-6.0f, -2.5f, 0.0f}};
}

auto StatefulSceneBuilder::createSponzas()
    -> void
{
    if (doNotIncludeSponza)
    {
        return;
    }

    auto const & model = assets->sponzaAssets.models[0];

    auto const positions = getSponzaPositions();

    auto const scaling = glm::scale(glm::mat4{1.0f}, {0.01f, 0.01f, 0.01f});

    for (auto const & position : positions)
    {
        auto const translation = glm::translate(glm::mat4{1.0f}, position);

        addBody(translation * scaling, model);
    }
}

auto StatefulSceneBuilder::getSponzaPositions() const
    -> std::vector<glm::vec3>
{
    return {
        {0.0f, -2.0f, -35.0f}};
}

auto StatefulSceneBuilder::createCameras()
    -> void
{
    createFrontCamera();

    createBackCamera();

    createRightCamera();
}

auto StatefulSceneBuilder::createFrontCamera()
     -> ape::Camera &
{
    using Frustum = ape::PerspectiveProjection::Frustum;

    auto const position = glm::vec3{0.0f, 0.0f, 3.0f};

    auto const direction = glm::vec3{0.0f, 0.0f, -1.0f};

    auto const up = glm::vec3{0.0f, 1.0f, 0.0f};

    auto const fieldOfView = glm::radians(45.0f);

    auto const aspectRatio = 1.0f; // Will be corrected upon viewport setup

    auto const placement = ape::CameraView::System{position, direction, up};

    auto const frustum = Frustum{fieldOfView, aspectRatio, 0.1f, 100.0f};

    return scene.addCamera({placement, frustum});
}

auto StatefulSceneBuilder::createBackCamera()
     -> ape::Camera &
{
    using Frustum = ape::PerspectiveProjection::Frustum;

    auto const position = glm::vec3{0.0f, 0.0f, -3.0f};

    auto const direction = glm::vec3{0.0f, 0.0f, 1.0f};

    auto const up = glm::vec3{0.0f, 1.0f, 0.0f};

    auto const fieldOfView = glm::radians(45.0f);

    auto const aspectRatio = 1.0f; // Will be corrected upon viewport setup

    auto const placement = ape::CameraView::System{position, direction, up};

    auto const frustum = Frustum{fieldOfView, aspectRatio, 0.1f, 100.0f};

    return scene.addCamera({placement, frustum});
}

auto StatefulSceneBuilder::createRightCamera()
     -> ape::Camera &
{
    using Frustum = ape::PerspectiveProjection::Frustum;

    auto const position = glm::vec3{5.0f, 0.0f, 0.0f};

    auto const direction = glm::vec3{-1.0f, 0.0f, 0.0f};

    auto const up = glm::vec3{0.0f, 1.0f, 0.0f};

    auto const fieldOfView = glm::radians(45.0f);

    auto const aspectRatio = 1.0f; // Will be corrected upon viewport setup

    auto const placement = ape::CameraView::System{position, direction, up};

    auto const frustum = Frustum{fieldOfView, aspectRatio, 0.1f, 100.0f};

    return scene.addCamera({placement, frustum});
}

auto StatefulSceneBuilder::createLightSystem()
    -> void
{
    createPointLights();
    
    createSpotLights();
    
    createDirectionalLights();
}

auto StatefulSceneBuilder::createPointLights()
    -> void
{
    auto const positions = getPointLampPositions();

    auto const colors = getPointLightColors();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const position = positions[i];

        auto const color = colors[i];

        auto & light = createPointLight(position, color, "PointLight_" + std::to_string(i));

        if ((i >= 2) && (i <= 9))
        {
            light.turnOff();
        }
    }
}

auto StatefulSceneBuilder::getPointLightColors()
    -> std::vector<ape::Light::Color>
{
    using Ambient = glm::vec3;

    using Diffuse = glm::vec3;
    
    using Specular = glm::vec3;

    return {
        {Ambient{0.1f, 0.1f, 0.1f}, Diffuse{0.4f, 0.3f, 0.9f}, Specular{0.3f, 0.2f, 1.0f}},
        {Ambient{0.1f, 0.1f, 0.1f}, Diffuse{0.3f, 0.8f, 0.2f}, Specular{0.4f, 0.9f, 0.2f}},
        {Ambient{0.1f, 0.1f, 0.1f}, Diffuse{0.0f, 0.0f, 1.0f}, Specular{0.2f, 0.2f, 1.0f}},
        {Ambient{0.1f, 0.1f, 0.1f}, Diffuse{0.8f, 0.9f, 0.2f}, Specular{1.0f, 1.0f, 0.2f}},
        {Ambient{0.1f, 0.1f, 0.1f}, Diffuse{0.4f, 0.3f, 0.6f}, Specular{1.0f, 0.7f, 0.8f}},
        {Ambient{0.1f, 0.1f, 0.1f}, Diffuse{0.4f, 0.3f, 0.6f}, Specular{1.0f, 0.7f, 0.8f}},
        {Ambient{0.1f, 0.1f, 0.1f}, Diffuse{0.4f, 0.3f, 0.6f}, Specular{1.0f, 0.7f, 0.8f}},
        {Ambient{0.1f, 0.1f, 0.1f}, Diffuse{0.8f, 0.3f, 0.3f}, Specular{0.9f, 0.3f, 0.3f}},
        {Ambient{0.1f, 0.1f, 0.1f}, Diffuse{0.3f, 0.8f, 1.0f}, Specular{0.3f, 0.7f, 0.9f}},
        {Ambient{0.1f, 0.1f, 0.1f}, Diffuse{1.0f, 1.0f, 1.0f}, Specular{1.0f, 1.0f, 1.0f}},
        {Ambient{0.1f, 0.1f, 0.1f}, Diffuse{1.0f, 1.0f, 1.0f}, Specular{1.0f, 1.0f, 1.0f}},
        {Ambient{0.1f, 0.1f, 0.1f}, Diffuse{0.4f, 0.3f, 0.6f}, Specular{1.0f, 0.7f, 0.8f}}};
}

auto StatefulSceneBuilder::createPointLight(
    glm::vec3 const & position,
    ape::Light::Color const & color,
    std::string name)
    -> ape::PointLight &
{
    auto const attenuation = ape::Attenuation{1.0f, 0.01f, 0.06f};

    auto const isTurnedOn = true;

    auto const isCastingShadow = true;

    auto & lightSystem = scene.getLightSystem();

    return lightSystem.point.emplace_back(
        std::move(name),
        position,
        attenuation,
        color,
        isTurnedOn,
        isCastingShadow);
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

        auto name = "SpotLight_" + std::to_string(i);

        createSpotLight(position, glm::normalize(-position), colors[i], std::move(name)); 
    }

    auto & light = createSpotLight(
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},
        colors.back(),
        "Headlight"); 

    light.turnOff();

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
        {Ambient{0.0f, 0.0f, 0.0f}, Diffuse{0.8f, 0.8f, 0.2f}, Specular{0.5f, 0.5f, 0.1f}},
        {Ambient{0.0f, 0.0f, 0.0f}, Diffuse{0.2f, 0.3f, 0.9f}, Specular{0.5f, 0.7f, 0.8f}},
        {Ambient{0.0f, 0.0f, 0.0f}, Diffuse{0.8f, 0.3f, 0.1f}, Specular{0.8f, 0.3f, 0.3f}},
        {Ambient{0.0f, 0.0f, 0.0f}, Diffuse{0.9f, 0.8f, 0.6f}, Specular{1.0f, 0.9f, 0.7f}}};
}

auto StatefulSceneBuilder::createSpotLight(
    glm::vec3 const & position,
    glm::vec3 const & direction,
    ape::Light::Color const & color,
    std::string name)
    -> ape::SpotLight &
{
    auto const cutoff = ape::SpotLight::CutoffAngle{glm::radians(10.0f), glm::radians(20.0f)};

    auto const attenuation = ape::Attenuation{1.0f, 0.005f, 0.062f};

    auto const isTurnedOn = true;

    auto const isCastingShadow = true;

    auto & lightSystem = scene.getLightSystem();

    return lightSystem.spot.emplace_back(
        std::move(name),
        position,
        direction,
        cutoff,
        attenuation,
        color,
        isTurnedOn,
        isCastingShadow);
}

auto StatefulSceneBuilder::createDirectionalLights()
    -> void
{
    auto const positions = getDirectionalLightDirections();

    for (auto i = 0; i < static_cast<int>(positions.size()); ++i)
    {
        auto const position = positions[i];

        createDirectionalLight(position, "DirectionalLight_" + std::to_string(i));
    }
}

auto StatefulSceneBuilder::getDirectionalLightDirections()
    -> std::vector<glm::vec3>
{
    return {glm::normalize(glm::vec3{0.0f, -1.0f, -1.0f})};
}

auto StatefulSceneBuilder::createDirectionalLight(glm::vec3 const & direction, std::string name)
    -> ape::DirectionalLight &
{
    auto const ambient = glm::vec3{0.0f, 0.0f, 0.0f};

    auto const diffuse = glm::vec3{0.3f, 0.3f, 0.3f};

    auto const specular = glm::vec3{0.2f, 0.2f, 0.2f};

    auto const color = ape::Light::Color{ambient, diffuse, specular};

    auto const isTurnedOn = false;

    auto const isCastingShadow = true;

    auto & lightSystem = scene.getLightSystem();

    return lightSystem.directional.emplace_back(
        std::move(name),
        direction,
        color,
        isTurnedOn,
        isCastingShadow);
}

auto StatefulSceneBuilder::addBody(glm::mat4 const & transformation, ape::Model const & model)
    -> ape::Body &
{
    auto body = ape::Body{model};

    ape::setTransformation(body, transformation);

    return scene.addBody(std::move(body));
}

auto StatefulSceneBuilder::createSynchronizers()
    -> void
{
    scene.cameraSpotLightSynchronizers.emplace_back(
        scene.getCameras().front(),
        *scene.playerFlashlight);

    for (auto i = 0u; i< scene.lamps.size(); ++i)
    {
        scene.bodyPointLightSynchronizers.emplace_back(
            *scene.lamps[i],
            scene.getLightSystem().point[i]);
    }

    for (auto i = 0u; i< scene.flashlights.size(); ++i)
    {
        scene.bodySpotLightSynchronizers.emplace_back(
            *scene.flashlights[i],
            scene.getLightSystem().spot[i]);
    }
}

} // unnamed namespace

RaveSceneBuilder::RaveSceneBuilder(RaveAssetCollection & assets)
    : RaveSceneBuilder{assets, true}
{
}

RaveSceneBuilder::RaveSceneBuilder(RaveAssetCollection & assets, bool const doNotIncludeSponza)
    : assets{&assets}
    , doNotIncludeSponza{doNotIncludeSponza}
{
}

auto RaveSceneBuilder::build() const
    -> RaveScene
{
    auto builder = StatefulSceneBuilder{*assets, doNotIncludeSponza};

    return builder.build();
}

} // namespace rave
