#include "Application.hpp"

#include <TestScene/SampleAssetBuilder.hpp>
#include <TestScene/SampleInputHandler.hpp>
#include <TestScene/SampleSceneBuilder.hpp>

#include <Ape/CameraSelector.hpp>
#include <Ape/Engine.hpp>
#include <Ape/GLFWGateway.hpp>
#include <Ape/SceneRenderer.hpp>
#include <Ape/StandardShaderProgram.hpp>

class Application::Impl
{

public:

    Impl()
        : window{gateway.createWindow("APE 3D engine", false)}
        , assets{createSampleAssets()}
        , scene{createSampleScene(assets)}
        , cameraSelector{scene}
        , renderer{cameraSelector, shader, {0.0f, 0.0f, 0.0f}}
        , inputHandler{window, cameraSelector, shader, scene}
        , engine{window, renderer, inputHandler}
    {
    }

    auto run()
        -> void
    {
        window.captureMouse();

        engine.run();
    }

private:

    ape::GLFWGateway gateway;

    ape::GLFWWindow window;

    SampleAssetCollection assets;

    SampleScene scene;

    ape::CameraSelector cameraSelector;

    ape::StandardShaderProgram shader;

    ape::SceneRenderer renderer;

    SampleInputHandler inputHandler;

    ape::Engine engine;

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
