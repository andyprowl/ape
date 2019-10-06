#include "Application.hpp"

#include <TestScene/SampleAssetBuilder.hpp>
#include <TestScene/SampleInputHandler.hpp>
#include <TestScene/SampleSceneBuilder.hpp>

#include <GlfwEngine/GLFWEngine.hpp>
#include <GlfwEngine/GLFWGateway.hpp>

#include <Rendering/SceneRenderer.hpp>
#include <Rendering/ShapeArrayObjectRenderer.hpp>
#include <Rendering/ShapeBufferObjectRenderer.hpp>
#include <Rendering/StandardShaderProgram.hpp>
#include <Rendering/WireframeShaderProgram.hpp>

#include <Scene/BodySelector.hpp>
#include <Scene/CameraSelector.hpp>

class Application::Impl
{

public:

    Impl()
        : window{gateway.createWindow("APE 3D GLFWEngine", {2000, 1000})}
        , assets{createSampleAssets()}
        , scene{createSampleScene(assets)}
        , standardShader{}
        , pickingShader{}
        , cameraSelector{scene}
        , bodyPicker{scene}
        , shapeArrayObjectRenderer{assets.shapes}
        , sceneRenderer{
            cameraSelector,
            bodyPicker,
            shapeArrayObjectRenderer,
            //shapeBufferObjectRenderer,
            standardShader,
            pickingShader,
            {0.0f, 0.0f, 0.0f}}
        , inputHandler{window, cameraSelector, bodyPicker, standardShader, sceneRenderer, scene}
        , GLFWEngine{window, sceneRenderer, inputHandler}
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

    SampleAssetCollection assets;

    SampleScene scene;

    ape::StandardShaderProgram standardShader;

    ape::WireframeShaderProgram pickingShader;

    ape::CameraSelector cameraSelector;

    ape::BodySelector bodyPicker;

    ape::ShapeArrayObjectRenderer shapeArrayObjectRenderer;

    ape::ShapeBufferObjectRenderer shapeBufferObjectRenderer;

    ape::SceneRenderer sceneRenderer;

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
