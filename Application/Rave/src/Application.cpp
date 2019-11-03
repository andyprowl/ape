#include "Application.hpp"

#include <Application/RaveCore/RaveAssetBuilder.hpp>
#include <Application/RaveCore/RaveInputHandler.hpp>
#include <Application/RaveCore/RaveSceneBuilder.hpp>
#include <Application/RaveCore/RaveSkyboxCollectionReader.hpp>

#include <Ape/GlfwEngine/GLFWEngine.hpp>
#include <Ape/GlfwEngine/GLFWGateway.hpp>
#include <Ape/Rendering/DepthShaderProgram.hpp>
#include <Ape/Rendering/EffectCollectionReader.hpp>
#include <Ape/Rendering/EffectSelector.hpp>
#include <Ape/Rendering/LineStyleProvider.hpp>
#include <Ape/Rendering/OutlinedBodyRenderer.hpp>
#include <Ape/Rendering/SceneRenderer.hpp>
#include <Ape/Rendering/ShapeArrayObjectRenderer.hpp>
#include <Ape/Rendering/ShapeBufferObjectRenderer.hpp>
#include <Ape/Rendering/SkyboxSelector.hpp>
#include <Ape/Rendering/SkyboxShaderProgram.hpp>
#include <Ape/Rendering/StandardBodyRenderer.hpp>
#include <Ape/Rendering/StandardShaderProgram.hpp>
#include <Ape/Rendering/WireframeBodyRenderer.hpp>
#include <Ape/Rendering/WireframeShaderProgram.hpp>
#include <Ape/Scene/BodySelector.hpp>
#include <Ape/Scene/CameraSelector.hpp>

namespace rave
{

class Application::Impl
{

public:

    explicit Impl(bool const enableDebugOutput)
        : gateway{4, 5, enableDebugOutput}
    {
        skyboxSelector.activateSkybox(3);

        effectSelector.activateEffect(5);
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

    ape::GLFWWindow window{gateway.createWindow("APE 3D GLFWEngine", {2000, 1000})};

    RaveAssetCollection assets{createSampleAssets()};

    RaveScene scene{createRaveScene(assets)};

    ape::StandardShaderProgram standardShader;

    ape::DepthShaderProgram depthShader;

    ape::WireframeShaderProgram wireframeShader;

    ape::SkyboxShaderProgram skyboxShader;

    ape::EffectCollection effectCollection{ape::EffectCollectionReader{}.read()};

    ape::EffectSelector effectSelector{effectCollection};

    ape::SkyboxCollection skyboxCollection{RaveSkyboxCollectionReader{}.read()};

    ape::SkyboxSelector skyboxSelector{skyboxCollection};

    std::unique_ptr<ape::ShapeArrayObjectRenderer> shapeRenderer{
        std::make_unique<ape::ShapeArrayObjectRenderer>(assets.shapes)};

    //std::unique_ptr<ape::ShapeBufferObjectRenderer> shapeRenderer{
    //  std::make_unique<ape::ShapeBufferObjectRenderer>()};

    ape::DepthBodyRenderer depthBodyRenderer{depthShader, *shapeRenderer};

    ape::StandardBodyRenderer standardBodyRenderer{standardShader, *shapeRenderer};

    ape::LineStyleProvider wireframeStyleProvider{{0.05f, {0.2f, 0.2f, 1.0f}}};

    ape::WireframeBodyRenderer wireframeBodyRenderer{
        wireframeShader,
        *shapeRenderer,
        wireframeStyleProvider};

    ape::OutlinedBodyRenderer outlinedBodyRenderer{standardBodyRenderer, wireframeBodyRenderer};

    ape::EffectRenderer effectRenderer{effectSelector};

    ape::SkyboxRenderer skyboxRenderer{skyboxShader, skyboxSelector};

    ape::CameraSelector cameraSelector{scene};

    ape::BodySelector bodyPicker{scene};

    ape::SceneRenderer sceneRenderer{
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
        {0.0f, 0.0f, 0.0f}};

    RaveInputHandler inputHandler{
        window,
        cameraSelector,
        skyboxSelector,
        effectSelector,
        bodyPicker,
        standardShader,
        wireframeStyleProvider,
        scene};

    ape::GLFWEngine engine{window, sceneRenderer, inputHandler};

};

Application::Application()
    : Application{false}
{
}

Application::Application(bool const enableDebugOutput)
    : impl{std::make_unique<Impl>(enableDebugOutput)}
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
