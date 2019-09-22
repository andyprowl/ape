#include "Application.hpp"

#include "SampleAssetBuilder.hpp"
#include "SampleInputHandler.hpp"
#include "SampleSceneBuilder.hpp"

#include <Ape/Engine.hpp>
#include <Ape/OpenGLGateway.hpp>
#include <Ape/SceneRenderer.hpp>
#include <Ape/StandardShaderProgram.hpp>

namespace
{

auto createAssets()
    -> SampleAssetCollection
{
    auto const builder = SampleAssetBuilder{};

    return builder.build();
}

auto createScene(SampleAssetCollection & assets)
    -> SampleScene
{
    auto const builder = SampleSceneBuilder{assets};

    return builder.build();
}

} // unnamed namespace

class Application::Impl
{

public:

    Impl()
        : window{gateway.createWindow("APE 3D engine", false)}
        , assets{createAssets()}
        , scene{createScene(assets)}
        , renderer{shader, {0.0f, 0.0f, 0.0f}}
        , inputHandler{window, scene, shader}
        , engine{window, scene, renderer, inputHandler}
    {
    }

    auto run()
        -> void
    {
        engine.run();
    }

private:

    ape::OpenGLGateway gateway;

    ape::Window window;

    SampleAssetCollection assets;

    SampleScene scene;

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
