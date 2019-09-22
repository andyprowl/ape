#include "Application.hpp"

#include "SampleAssetBuilder.hpp"
#include "SampleSceneBuilder.hpp"

#include <Ape/OpenGLGateway.hpp>

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
        , inputHandler{window, scene}
        , engine{window, scene, inputHandler}
    {
    }

    auto run()
        -> void
    {
        engine.run();
    }

private:

    OpenGLGateway gateway;

    Window window;

    SampleAssetCollection assets;

    SampleScene scene;

    SampleInputHandler inputHandler;

    Engine engine;

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
