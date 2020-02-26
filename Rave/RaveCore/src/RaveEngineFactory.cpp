#include <Rave/RaveCore/RaveEngineFactory.hpp>

#include <Rave/RaveCore/RaveAssetCollection.hpp>
#include <Rave/RaveCore/RaveEffectCollectionReader.hpp>
#include <Rave/RaveCore/RaveInputHandler.hpp>
#include <Rave/RaveCore/RaveSceneBuilder.hpp>
#include <Rave/RaveCore/RaveSkyboxCollectionReader.hpp>

#include <Ape/Rendering/Effect/EffectCollectionPopulator.hpp>
#include <Ape/Rendering/Effect/EffectSelector.hpp>
#include <Ape/Rendering/Lighting/LightSystemUniformSetter.hpp>
#include <Ape/Rendering/Lighting/LightSystemViewUniformSetter.hpp>
#include <Ape/Rendering/Lighting/MaterialSetUniformSetter.hpp>
#include <Ape/Rendering/Lighting/MonoDepthShaderProgram.hpp>
#include <Ape/Rendering/Lighting/OmniDepthCubeShaderProgram.hpp>
#include <Ape/Rendering/Lighting/OmniDepthFlatShaderProgram.hpp>
#include <Ape/Rendering/Lighting/BlinnPhongBodyRenderer.hpp>
#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>
#include <Ape/Rendering/Rendering/BodyBoundsShaderProgram.hpp>
#include <Ape/Rendering/Rendering/OutlinedBodyRenderer.hpp>
#include <Ape/Rendering/Rendering/SceneRenderer.hpp>
#include <Ape/World/Scene/BodySelector.hpp>
#include <Ape/World/Scene/CameraSelector.hpp>
#include <Ape/World/Shape/ShapeArrayObjectDrawer.hpp>
#include <Ape/World/Shape/ShapeBufferObjectDrawer.hpp>
#include <Ape/World/Shape/SharedArrayObjectDrawer.hpp>
#include <Ape/World/Shape/SharedArrayBufferObjectDrawer.hpp>
#include <Ape/World/Shape/SharedBufferObjectDrawer.hpp>
#include <Ape/Rendering/Skybox/SkyboxSelector.hpp>
#include <Ape/Rendering/Skybox/SkyboxShaderProgram.hpp>
#include <Ape/Rendering/Wireframe/LineStyleProvider.hpp>
#include <Ape/Rendering/Wireframe/WireframeBodyRenderer.hpp>
#include <Ape/Rendering/Wireframe/WireframeShaderProgram.hpp>

namespace rave
{

namespace
{

class RenderingShaders
{

public:

    ape::BlinnPhongShaderProgram blinnPhongShader;

    ape::MonoDepthShaderProgram monoDepthShader;

    ape::OmniDepthCubeShaderProgram omniDepthCubeShader;

    ape::OmniDepthFlatShaderProgram omniDepthFlatShader;

    ape::WireframeShaderProgram wireframeShader;

    ape::BodyBoundsShaderProgram boundsShader;

    ape::SkyboxShaderProgram skyboxShader;

};

class RaveResources
{

public:

    explicit RaveResources(bool excludeSponza)
        : assets{excludeSponza}
        , effectCollection{RaveEffectCollectionReader{}.read()}
        , skyboxCollection{RaveSkyboxCollectionReader{}.read()}
    {
    }

public:

    RaveAssetCollection assets;

    ape::EffectCollection effectCollection;

    ape::SkyboxCollection skyboxCollection;

};

} // unnamed namespace

class RaveEngineFactory::EngineObjects
{

public:

    explicit EngineObjects(bool const excludeSponza)
        : shaders{}
        , resources{excludeSponza}
        , scene{createRaveScene(resources.assets, excludeSponza)}
        , lightSystemView{scene.getLightSystem(), basix::Size2d<int>{1024, 1024}}
        , lightSystemSetter{scene.getLightSystem(), shaders.blinnPhongShader.lightSystem}
        , lightSystemViewSetter{lightSystemView, shaders.blinnPhongShader.lightSystemView}
        , materialSetSetter{resources.assets.getMaterials(), shaders.blinnPhongShader.materialSet}
        , effectSelector{resources.effectCollection}
        , skyboxSelector{resources.skyboxCollection}
        , bodyPicker{scene}
        , wireframeStyleProvider{{0.05f, {0.2f, 0.2f, 1.0f}}}
    {
        skyboxSelector.activateSkybox(5);

        effectSelector.activateEffect(4);
    }

public:

    RenderingShaders shaders;

    RaveResources resources;

    RaveScene scene;

    ape::LightSystemView lightSystemView;

    ape::LightSystemUniformSetter lightSystemSetter;

    ape::LightSystemViewUniformSetter lightSystemViewSetter;

    ape::MaterialSetUniformSetter materialSetSetter;

    ape::EffectSelector effectSelector;

    ape::SkyboxSelector skyboxSelector;

    ape::BodySelector bodyPicker;

    ape::LineStyleProvider wireframeStyleProvider;

};

RaveEngineFactory::RaveEngineFactory(bool const excludeSponza)
    : engineObjects{std::make_unique<EngineObjects>(excludeSponza)}
{
}

RaveEngineFactory::RaveEngineFactory(RaveEngineFactory && rhs) noexcept = default;

auto RaveEngineFactory::operator = (RaveEngineFactory && rhs) noexcept
    -> RaveEngineFactory & = default;

RaveEngineFactory::~RaveEngineFactory() = default;

auto RaveEngineFactory::getScene()
    -> ape::Scene &
{
    return engineObjects->scene;
}

auto RaveEngineFactory::makeSceneRenderer(
    ape::Window & window,
    ape::CameraSelector & cameraSelector)
    -> std::unique_ptr<ape::SceneRenderer>
{
    // IMPORTANT: fallback VAO in renderer as well as VAOs in ShapeDrawer must be created in the
    // corresponding rendering context! Also, the flat quad VAO for rendering of offscreen texture
    // must be created in the corresponding renderer context.
    window.makeCurrent();

    auto shadowMapping = std::make_unique<ape::ShadowMapping>(
        engineObjects->lightSystemView,
        basix::Size2d<int>{1024, 1024});

    auto & shapes = engineObjects->resources.assets.getShapes();

    // Using a VAO per shape seems to make performance worse...
    // auto shapeDrawer = std::make_unique<ape::ShapeArrayObjectDrawer>(shapes);
    // auto shapeDrawer = std::make_unique<ape::ShapeBufferObjectDrawer>(shapes);
    // auto shapeDrawer = std::make_unique<ape::SharedArrayObjectDrawer>(shapes);
    // auto shapeDrawer = std::make_unique<ape::SharedBufferObjectDrawer>(shapes);
    auto shapeDrawer = std::make_unique<ape::SharedArrayBufferObjectDrawer>(shapes);

    auto depthBodyRenderer = ape::DepthBodyRenderer{
        {engineObjects->shaders.monoDepthShader, *shapeDrawer},
        {engineObjects->shaders.omniDepthCubeShader, *shapeDrawer},
        {engineObjects->shaders.omniDepthFlatShader, *shapeDrawer}};

    auto standardBodyRenderer = ape::BlinnPhongBodyRenderer{
        engineObjects->shaders.blinnPhongShader,
        engineObjects->lightSystemSetter,
        engineObjects->lightSystemViewSetter,
        engineObjects->materialSetSetter,
        *shapeDrawer};

    auto wireframeBodyRenderer = ape::WireframeBodyRenderer{
        engineObjects->shaders.wireframeShader,
        *shapeDrawer,
        engineObjects->wireframeStyleProvider};

    auto outlinedBodyRenderer = ape::OutlinedBodyRenderer{
        standardBodyRenderer,
        wireframeBodyRenderer};

    auto bodyBoundsRenderer = ape::BodyBoundsRenderer{engineObjects->shaders.boundsShader};

    auto skyboxRenderer = ape::SkyboxRenderer{
        engineObjects->shaders.skyboxShader,
        engineObjects->skyboxSelector};

    auto effectRenderer = ape::EffectRenderer{engineObjects->effectSelector};

    auto const backgroundColor = glm::vec3{0.0f, 0.0f, 0.0f};

    return std::make_unique<ape::SceneRenderer>(
        ape::SceneRenderer::RendererSet{
            std::move(depthBodyRenderer),
            std::move(standardBodyRenderer),
            std::move(wireframeBodyRenderer),
            std::move(outlinedBodyRenderer),
            std::move(bodyBoundsRenderer),
            std::move(skyboxRenderer),
            std::move(effectRenderer)},
        std::move(shapeDrawer),
        std::move(shadowMapping),
        cameraSelector,
        engineObjects->bodyPicker,
        window,
        ape::Viewport{{0, 0}, window.getSize()},
        backgroundColor);
}

auto RaveEngineFactory::makeInputHandler(
    ape::SceneRenderer & sceneRenderer,
    ape::CameraSelector & cameraSelector)
    -> std::unique_ptr<RaveInputHandler>
{
    return std::make_unique<RaveInputHandler>(
        sceneRenderer.getTargetSurface(),
        sceneRenderer,
        cameraSelector,
        engineObjects->skyboxSelector,
        engineObjects->effectSelector,
        engineObjects->bodyPicker,
        engineObjects->shaders.blinnPhongShader,
        engineObjects->wireframeStyleProvider,
        engineObjects->scene);
}

} // namespace rave
