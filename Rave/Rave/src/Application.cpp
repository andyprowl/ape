#include "Application.hpp"

#include <Rave/RaveCore/RaveAssetBuilder.hpp>
#include <Rave/RaveCore/RaveEffectCollectionReader.hpp>
#include <Rave/RaveCore/RaveInputHandler.hpp>
#include <Rave/RaveCore/RaveSceneBuilder.hpp>
#include <Rave/RaveCore/RaveSkyboxCollectionReader.hpp>

#include <Ape/Engine/GlfwEngine/GlfwEngine.hpp>
#include <Ape/Engine/GlfwEngine/GlfwGateway.hpp>
#include <Ape/Rendering/Effect/EffectCollectionPopulator.hpp>
#include <Ape/Rendering/Effect/EffectSelector.hpp>
#include <Ape/Rendering/Lighting/LightSystemUniformSetter.hpp>
#include <Ape/Rendering/Lighting/LightSystemViewUniformSetter.hpp>
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

class SharedResources
{

public:

    explicit SharedResources(bool doNotIncludeSponza)
        : assets{createRaveAssets(doNotIncludeSponza)}
        , effectCollection{RaveEffectCollectionReader{}.read()}
        , skyboxCollection{RaveSkyboxCollectionReader{}.read()}
    {
    }

public:

    RenderingShaders shaders;

    RaveAssetCollection assets;

    ape::EffectCollection effectCollection;

    ape::SkyboxCollection skyboxCollection;

};

} // unnamed namespace

class Application::Impl
{

public:

    Impl(bool const enableDebugOutput, bool const doNotIncludeSponza)
        : gateway{4, 5, enableDebugOutput}
        , window{gateway.createWindow("Rave", basix::Size<int>{1920, 1080})}
        , resources{doNotIncludeSponza}
        , scene{createRaveScene(resources.assets, doNotIncludeSponza)}
        , effectSelector{resources.effectCollection}
        , skyboxSelector{resources.skyboxCollection}
        , cameraSelector{scene}
        , bodyPicker{scene}
        , lightSystemView{scene.getLightSystem(), basix::Size<int>{1024, 1024}}
        , lightSystemSetter{scene.getLightSystem(), resources.shaders.blinnPhongShader.lightSystem}
        , lightSystemViewSetter{lightSystemView, resources.shaders.blinnPhongShader.lightSystemView}
        , wireframeStyleProvider{{0.05f, {0.2f, 0.2f, 1.0f}}}
        , sceneRenderer{makeSceneRenderer()}
        , inputHandler{makeInputHandler()}
        , engine{window, *sceneRenderer, *inputHandler}
    {
        skyboxSelector.activateSkybox(5);

        effectSelector.activateEffect(4);
    }

    auto run()
        -> void
    {
        window.captureMouse();

        inputHandler->getCameraManipulator().activate();

        engine.run();
    }

private:

    auto makeSceneRenderer()
        -> std::unique_ptr<ape::SceneRenderer>
    {
        auto shadowMapping = std::make_unique<ape::ShadowMapping>(
            lightSystemView,
            basix::Size<int>{1024, 1024});

        auto & shapes = resources.assets.shapes;

        // Using a VAO per shape seems to make performance worse...
        // auto shapeDrawer = std::make_unique<ape::ShapeArrayObjectDrawer>(shapes);
        // auto shapeDrawer = std::make_unique<ape::ShapeBufferObjectDrawer>(shapes);
        // auto shapeDrawer = std::make_unique<ape::SharedArrayObjectDrawer>(shapes);
        // auto shapeDrawer = std::make_unique<ape::SharedBufferObjectDrawer>(shapes);
        auto shapeDrawer = std::make_unique<ape::SharedArrayBufferObjectDrawer>(shapes);

        auto depthBodyRenderer = ape::DepthBodyRenderer{
            {resources.shaders.monoDepthShader, *shapeDrawer},
            {resources.shaders.omniDepthCubeShader, *shapeDrawer},
            {resources.shaders.omniDepthFlatShader, *shapeDrawer}};

        auto standardBodyRenderer = ape::BlinnPhongBodyRenderer{
            resources.shaders.blinnPhongShader,
            lightSystemSetter,
            lightSystemViewSetter,
            *shapeDrawer};

         auto wireframeBodyRenderer = ape::WireframeBodyRenderer{
             resources.shaders.wireframeShader,
             *shapeDrawer, wireframeStyleProvider};

         auto outlinedBodyRenderer = ape::OutlinedBodyRenderer{
             standardBodyRenderer,
             wireframeBodyRenderer};

         auto bodyBoundsRenderer = ape::BodyBoundsRenderer{resources.shaders.boundsShader};

         auto skyboxRenderer = ape::SkyboxRenderer{resources.shaders.skyboxShader, skyboxSelector};

         auto effectRenderer = ape::EffectRenderer{effectSelector};

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
            bodyPicker,
            window,
            ape::Viewport{{0, 0}, window.getSize()},
            backgroundColor);
    }

    auto makeInputHandler()
        -> std::unique_ptr<RaveInputHandler>
    {
        return std::make_unique<RaveInputHandler>(
            window,
            *sceneRenderer,
            cameraSelector,
            skyboxSelector,
            effectSelector,
            bodyPicker,
            resources.shaders.blinnPhongShader,
            wireframeStyleProvider,
            scene);
    }

private:

    ape::GlfwGateway gateway;

    ape::GlfwWindow window;

    SharedResources resources;

    RaveScene scene;

    ape::EffectSelector effectSelector;

    ape::SkyboxSelector skyboxSelector;

    ape::CameraSelector cameraSelector;

    ape::BodySelector bodyPicker;

    ape::LightSystemView lightSystemView;

    ape::LineStyleProvider wireframeStyleProvider;

    ape::LightSystemUniformSetter lightSystemSetter;

    ape::LightSystemViewUniformSetter lightSystemViewSetter;

    std::unique_ptr<ape::SceneRenderer> sceneRenderer;

    std::unique_ptr<RaveInputHandler> inputHandler;

    ape::GlfwEngine engine;

};

Application::Application()
    : Application{false, false}
{
}

Application::Application(bool const enableDebugOutput, bool const doNotIncludeSponza)
    : impl{std::make_unique<Impl>(enableDebugOutput, doNotIncludeSponza)}
{
}

Application::Application(Application && rhs) noexcept = default;

auto Application::operator = (Application && rhs) noexcept
    -> Application & = default;

Application::~Application() = default;

auto Application::run()
    -> void
{
    return impl->run();
}

} // namespace rave
