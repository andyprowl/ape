#include "Application.hpp"

#include <TestScene/SampleAssetBuilder.hpp>
#include <TestScene/SampleInputHandler.hpp>
#include <TestScene/SampleSceneBuilder.hpp>

#include <GLFWWindow/GLFWGateway.hpp>

#include <Core/CameraSelector.hpp>
#include <Core/Engine.hpp>
#include <Core/OpenGLLoader.hpp>
#include <Core/SceneRenderer.hpp>
#include <Core/StandardShaderProgram.hpp>

class Application::Impl
{

public:

    Impl()
        : window{gateway.createWindow("APE 3D engine", false)}
        , loader{true, true}
        , assets{createSampleAssets()}
        , scene{createSampleScene(assets)}
        , cameraSelector{scene}
        , context{ape::RenderingPolicy::useArrayObjects}
        , renderer{context, cameraSelector, shader, {0.0f, 0.0f, 0.0f}}
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

    ape::OpenGLLoader loader;

    SampleAssetCollection assets;

    SampleScene scene;

    ape::CameraSelector cameraSelector;

    ape::StandardShaderProgram shader;

    ape::RenderingContext context;

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
