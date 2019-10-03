#include "Application.hpp"

#include <TestScene/SampleAssetBuilder.hpp>
#include <TestScene/SampleInputHandler.hpp>
#include <TestScene/SampleSceneBuilder.hpp>

#include <GlfwEngine/GLFWEngine.hpp>
#include <GlfwEngine/GLFWGateway.hpp>

#include <Engine/BodySelector.hpp>
#include <Engine/CameraSelector.hpp>
#include <Engine/OpenGLLoader.hpp>
#include <Engine/WireframeShaderProgram.hpp>
#include <Engine/SceneRenderer.hpp>
#include <Engine/StandardShaderProgram.hpp>

class Application::Impl
{

public:

    Impl()
        : window{gateway.createWindow("APE 3D GLFWEngine", {2000, 1000})}
        , loader{true, true}
        , assets{createSampleAssets()}
        , scene{createSampleScene(assets)}
        , standardShader{}
        , pickingShader{}
        , cameraSelector{scene}
        , bodyPicker{scene}
        , context{ape::RenderingPolicy::useArrayObjects}
        , renderer{
            context,
            cameraSelector,
            bodyPicker,
            standardShader,
            pickingShader,
            {0.0f, 0.0f, 0.0f}}
        , inputHandler{window, cameraSelector, bodyPicker, standardShader, scene}
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

    ape::StandardShaderProgram standardShader;

    ape::WireframeShaderProgram pickingShader;

    ape::CameraSelector cameraSelector;

    ape::BodySelector bodyPicker;

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
