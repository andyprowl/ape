#include "Application.hpp"

#include <Rave/RaveCore/RaveAssetBuilder.hpp>
#include <Rave/RaveCore/RaveEffectCollectionReader.hpp>
#include <Rave/RaveCore/RaveInputHandler.hpp>
#include <Rave/RaveCore/RaveSceneBuilder.hpp>
#include <Rave/RaveCore/RaveSkyboxCollectionReader.hpp>

#include <Ape/Effect/EffectCollectionPopulator.hpp>
#include <Ape/Effect/EffectSelector.hpp>
#include <Ape/GlfwEngine/GLFWEngine.hpp>
#include <Ape/GlfwEngine/GLFWGateway.hpp>
#include <Ape/Lighting/MonoDepthShaderProgram.hpp>
#include <Ape/Lighting/OmniDepthShaderProgram.hpp>
#include <Ape/Lighting/LightingBodyRenderer.hpp>
#include <Ape/Lighting/LightingShaderProgram.hpp>
#include <Ape/Rendering/OutlinedBodyRenderer.hpp>
#include <Ape/Rendering/SceneRenderer.hpp>
#include <Ape/Scene/BodySelector.hpp>
#include <Ape/Scene/CameraSelector.hpp>
#include <Ape/Shape/ShapeArrayObjectDrawer.hpp>
#include <Ape/Shape/ShapeBufferObjectDrawer.hpp>
#include <Ape/Skybox/SkyboxSelector.hpp>
#include <Ape/Skybox/SkyboxShaderProgram.hpp>
#include <Ape/Wireframe/LineStyleProvider.hpp>
#include <Ape/Wireframe/WireframeBodyRenderer.hpp>
#include <Ape/Wireframe/WireframeShaderProgram.hpp>

namespace rave
{

class Application::Impl
{

public:

    Impl(bool const enableDebugOutput, bool const doNotIncludeSponza)
        : gateway{4, 5, enableDebugOutput}
        , window{gateway.createWindow("APE 3D GLFWEngine", {2000, 1000})}
        , assets{createRaveAssets(doNotIncludeSponza)}
        , scene{createRaveScene(assets, doNotIncludeSponza)}
        , effectCollection{RaveEffectCollectionReader{}.read()}
        , effectSelector{effectCollection}
        , skyboxCollection{RaveSkyboxCollectionReader{}.read()}
        , skyboxSelector{skyboxCollection}
        , shapeRenderer{std::make_unique<ape::ShapeArrayObjectDrawer>(assets.shapes)}
     // , shapeRenderer{std::make_unique<ape::ShapeBufferObjectDrawer>()}
        , depthBodyRenderer{{monoDepthShader, *shapeRenderer}, {omniDepthShader, *shapeRenderer}}
        , standardBodyRenderer{standardShader, *shapeRenderer}
        , wireframeStyleProvider{{0.05f, {0.2f, 0.2f, 1.0f}}}
        , wireframeBodyRenderer{wireframeShader, *shapeRenderer, wireframeStyleProvider}
        , outlinedBodyRenderer{standardBodyRenderer, wireframeBodyRenderer}
        , effectRenderer{effectSelector}
        , skyboxRenderer{skyboxShader, skyboxSelector}
        , cameraSelector{scene}
        , bodyPicker{scene}
        , sceneRenderer{
            std::move(shapeRenderer),
            std::move(depthBodyRenderer),
            std::move(standardBodyRenderer),
            std::move(wireframeBodyRenderer),
            std::move(outlinedBodyRenderer),
            std::move(skyboxRenderer),
            std::move(effectRenderer),
            cameraSelector,
            bodyPicker,
            window,
            ape::Viewport{{0, 0}, window.getSize()},
            {0.0f, 0.0f, 0.0f}}
        , inputHandler{
            window,
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

        engine.start();
    }

private:

    ape::GLFWGateway gateway;

    ape::GLFWWindow window;

    RaveAssetCollection assets;

    RaveScene scene;

    ape::LightingShaderProgram standardShader;

    ape::MonoDepthShaderProgram monoDepthShader;

    ape::OmniDepthShaderProgram omniDepthShader;

    ape::WireframeShaderProgram wireframeShader;

    ape::SkyboxShaderProgram skyboxShader;

    ape::EffectCollection effectCollection;

    ape::EffectSelector effectSelector;

    ape::SkyboxCollection skyboxCollection;

    ape::SkyboxSelector skyboxSelector;

    std::unique_ptr<ape::ShapeDrawer> shapeRenderer;

    ape::DepthBodyRenderer depthBodyRenderer;

    ape::LightingBodyRenderer standardBodyRenderer;

    ape::LineStyleProvider wireframeStyleProvider;

    ape::WireframeBodyRenderer wireframeBodyRenderer;

    ape::OutlinedBodyRenderer outlinedBodyRenderer;

    ape::EffectRenderer effectRenderer;

    ape::SkyboxRenderer skyboxRenderer;

    ape::CameraSelector cameraSelector;

    ape::BodySelector bodyPicker;

    ape::SceneRenderer sceneRenderer;;

    RaveInputHandler inputHandler;

    ape::GLFWEngine engine;

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
