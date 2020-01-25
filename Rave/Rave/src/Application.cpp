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
#include <Ape/World/Shape/SharedBufferObjectDrawer.hpp>
#include <Ape/Rendering/Skybox/SkyboxSelector.hpp>
#include <Ape/Rendering/Skybox/SkyboxShaderProgram.hpp>
#include <Ape/Rendering/Wireframe/LineStyleProvider.hpp>
#include <Ape/Rendering/Wireframe/WireframeBodyRenderer.hpp>
#include <Ape/Rendering/Wireframe/WireframeShaderProgram.hpp>

namespace rave
{

class Application::Impl
{

public:

    Impl(bool const enableDebugOutput, bool const doNotIncludeSponza)
        : gateway{4, 5, enableDebugOutput}
        , window{gateway.createWindow("Rave", {1920, 1080})}
        , assets{createRaveAssets(doNotIncludeSponza)}
        , scene{createRaveScene(assets, doNotIncludeSponza)}
        , effectCollection{RaveEffectCollectionReader{}.read()}
        , effectSelector{effectCollection}
        , skyboxCollection{RaveSkyboxCollectionReader{}.read()}
        , skyboxSelector{skyboxCollection}
     // Using a VAO per shape seems to make performance worse...
     // , shapeRenderer{std::make_unique<ape::ShapeArrayObjectDrawer>(assets.shapes)}
     // , shapeRenderer{std::make_unique<ape::ShapeBufferObjectDrawer>(assets.shapes)}
     // , shapeRenderer{std::make_unique<ape::SharedArrayObjectDrawer>(assets.shapes)}
        , shapeRenderer{std::make_unique<ape::SharedBufferObjectDrawer>(assets.shapes)}
        , depthBodyRenderer{
            {monoDepthShader, *shapeRenderer},
            {omniDepthCubeShader, *shapeRenderer},
            {omniDepthFlatShader, *shapeRenderer}}
        , standardBodyRenderer{standardShader, *shapeRenderer}
        , wireframeStyleProvider{{0.05f, {0.2f, 0.2f, 1.0f}}}
        , wireframeBodyRenderer{wireframeShader, *shapeRenderer, wireframeStyleProvider}
        , outlinedBodyRenderer{standardBodyRenderer, wireframeBodyRenderer}
        , bodyBoundsRenderer{boundsShader}
        , skyboxRenderer{skyboxShader, skyboxSelector}
        , effectRenderer{effectSelector}
        , cameraSelector{scene}
        , bodyPicker{scene}
        , sceneRenderer{
            std::move(shapeRenderer),
            ape::SceneRenderer::RendererSet{
                std::move(depthBodyRenderer),
                std::move(standardBodyRenderer),
                std::move(wireframeBodyRenderer),
                std::move(outlinedBodyRenderer),
                std::move(bodyBoundsRenderer),
                std::move(skyboxRenderer),
                std::move(effectRenderer)},
            cameraSelector,
            bodyPicker,
            window,
            ape::Viewport{{0, 0}, window.getSize()},
            {0.0f, 0.0f, 0.0f}}
        , inputHandler{
            window,
            sceneRenderer,
            cameraSelector,
            skyboxSelector,
            effectSelector,
            bodyPicker,
            standardShader,
            wireframeStyleProvider,
            scene}
        , engine{window, sceneRenderer, inputHandler}
    {
        skyboxSelector.activateSkybox(5);

        effectSelector.activateEffect(4);
    }

    auto run()
        -> void
    {
        window.captureMouse();

        inputHandler.getCameraManipulator().activate();

        engine.run();
    }

private:

    ape::GlfwGateway gateway;

    ape::GlfwWindow window;

    RaveAssetCollection assets;

    RaveScene scene;

    ape::BlinnPhongShaderProgram standardShader;

    ape::MonoDepthShaderProgram monoDepthShader;

    ape::OmniDepthCubeShaderProgram omniDepthCubeShader;

    ape::OmniDepthFlatShaderProgram omniDepthFlatShader;

    ape::WireframeShaderProgram wireframeShader;

    ape::BodyBoundsShaderProgram boundsShader;

    ape::SkyboxShaderProgram skyboxShader;

    ape::EffectCollection effectCollection;

    ape::EffectSelector effectSelector;

    ape::SkyboxCollection skyboxCollection;

    ape::SkyboxSelector skyboxSelector;

    std::unique_ptr<ape::ShapeDrawer> shapeRenderer;

    ape::DepthBodyRenderer depthBodyRenderer;

    ape::BlinnPhongBodyRenderer standardBodyRenderer;

    ape::LineStyleProvider wireframeStyleProvider;

    ape::WireframeBodyRenderer wireframeBodyRenderer;

    ape::OutlinedBodyRenderer outlinedBodyRenderer;

    ape::BodyBoundsRenderer bodyBoundsRenderer;

    ape::SkyboxRenderer skyboxRenderer;

    ape::EffectRenderer effectRenderer;

    ape::CameraSelector cameraSelector;

    ape::BodySelector bodyPicker;

    ape::SceneRenderer sceneRenderer;;

    RaveInputHandler inputHandler;

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
