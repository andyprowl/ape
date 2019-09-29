#include "Application.hpp"

#include <TestScene/SampleAssetBuilder.hpp>
#include <TestScene/SampleInputHandler.hpp>
#include <TestScene/SampleSceneBuilder.hpp>

#include <GlfwBinding/GLFWEngine.hpp>
#include <GlfwBinding/GLFWGateway.hpp>

#include <Core/CameraSelector.hpp>
#include <Core/OpenGLLoader.hpp>
#include <Core/SceneRenderer.hpp>
#include <Core/StandardShaderProgram.hpp>

class Application::Impl
{

public:

    Impl()
        : window{gateway.createWindow("APE 3D GLFWEngine", {2000, 1000})}
        , loader{true, true}
        , assets{createSampleAssets()}
        , scene{createSampleScene(assets)}
        , shader{}
        , cameraSelector{scene}
        , context{ape::RenderingPolicy::useArrayObjects}
        , renderer{context, cameraSelector, shader, {0.0f, 0.0f, 0.0f}}
        , inputHandler{window, cameraSelector, shader, scene}
        , GLFWEngine{window, renderer, inputHandler}
    {
    }

    auto run()
        -> void
    {
        window.captureMouse();

        inputHandler.getCameraManipulator().activate();

        GLFWEngine.start();
    }

private:

    ape::GLFWGateway gateway;

    ape::GLFWWindow window;

    ape::OpenGLLoader loader;

    SampleAssetCollection assets;

    SampleScene scene;

    ape::StandardShaderProgram shader;

    ape::CameraSelector cameraSelector;

    ape::RenderingContext context;

    ape::SceneRenderer renderer;

    SampleInputHandler inputHandler;

    ape::GLFWEngine GLFWEngine;

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
