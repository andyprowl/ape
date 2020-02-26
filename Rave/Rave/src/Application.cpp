#include "Application.hpp"

#include <Rave/RaveCore/RaveEngineFactory.hpp>

#include <Ape/Engine/GlfwEngine/GlfwEngine.hpp>
#include <Ape/Engine/GlfwEngine/GlfwGateway.hpp>

namespace rave
{

class Application::Impl
{

public:

    Impl(bool const enableDebugOutput, bool const excludeSponza)
        : gateway{4, 5, enableDebugOutput}
        , window{gateway.createWindow("Rave", basix::Size2d<int>{1920, 1080})}
        , engineFactory{excludeSponza}
        , engine{engineFactory.makeEngine<ape::GlfwEngine>(window)}
    {
    }

    auto run()
        -> void
    {
        activateCameraManipulator();

        window.captureMouse();

        engine->run();
    }

private:

    auto activateCameraManipulator()
        -> void
    {
        auto & inputHandler = dynamic_cast<ape::StandardInputHandler &>(engine->getInputHandler());

        inputHandler.getCameraManipulator().activate();
    }

private:

    ape::GlfwGateway gateway;

    ape::GlfwWindow window;

    RaveEngineFactory engineFactory;

    std::unique_ptr<ape::Engine> engine;

};

Application::Application()
    : Application{false, false}
{
}

Application::Application(bool const enableDebugOutput, bool const excludeSponza)
    : impl{std::make_unique<Impl>(enableDebugOutput, excludeSponza)}
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
