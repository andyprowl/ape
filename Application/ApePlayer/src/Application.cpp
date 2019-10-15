#include "Application.hpp"

#include <TestScene/SampleAssetBuilder.hpp>
#include <TestScene/SampleInputHandler.hpp>
#include <TestScene/SampleSceneBuilder.hpp>

#include <GlfwEngine/GLFWEngine.hpp>
#include <GlfwEngine/GLFWGateway.hpp>

#include <Rendering/DepthShaderProgram.hpp>
#include <Rendering/LineStyleProvider.hpp>
#include <Rendering/OutlinedBodyRenderer.hpp>
#include <Rendering/SceneRenderer.hpp>
#include <Rendering/ShapeArrayObjectRenderer.hpp>
#include <Rendering/ShapeBufferObjectRenderer.hpp>
#include <Rendering/StandardBodyRenderer.hpp>
#include <Rendering/StandardShaderProgram.hpp>
#include <Rendering/WireframeBodyRenderer.hpp>
#include <Rendering/WireframeShaderProgram.hpp>

#include <Scene/BodySelector.hpp>
#include <Scene/CameraSelector.hpp>

class Application::Impl
{

public:

    auto run()
        -> void
    {
        window.captureMouse();

        inputHandler.getCameraManipulator().activate();

        GLFWEngine.start();
    }

private:

    ape::GLFWGateway gateway{4, 5};

    ape::GLFWWindow window{gateway.createWindow("APE 3D GLFWEngine", {2000, 1000})};

    SampleAssetCollection assets{createSampleAssets()};

    SampleScene scene{createSampleScene(assets)};

    ape::StandardShaderProgram standardShader;

    ape::DepthShaderProgram depthShader;

    ape::WireframeShaderProgram wireframeShader;

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

    ape::CameraSelector cameraSelector{scene};

    ape::BodySelector bodyPicker{scene};

    ape::SceneRenderer sceneRenderer{
        std::move(shapeRenderer),
        std::move(depthBodyRenderer),
        std::move(standardBodyRenderer),
        std::move(wireframeBodyRenderer),
        std::move(outlinedBodyRenderer),
        cameraSelector,
        bodyPicker,
        ape::Viewport{{0, 0}, window.getSize()},
        {0.0f, 0.0f, 0.0f}};

    SampleInputHandler inputHandler{
        window,
        cameraSelector,
        bodyPicker,
        standardShader,
        wireframeStyleProvider,
        scene};

    ape::GLFWEngine GLFWEngine{window, sceneRenderer, inputHandler};

};

Application::Application()
    : impl{std::make_unique<Impl>()}
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
