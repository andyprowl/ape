#include <Ape/Engine/QtEngine/QtGateway.hpp>

#include <Ape/Engine/Initialization/OpenGLLoader.hpp>

namespace ape::qt
{

class QtGateway::Impl
{

public:

    explicit Impl(bool const enableDebugOutput)
        : loader{false, enableDebugOutput}
    {
    }

    auto createWindow()
        -> QtWindow *
    {
        return new QtWindow{loader};
    }

    auto createWindow(QWidget & parent)
        -> QtWindow *
    {
        return new QtWindow{loader, parent};
    }

private:

    OpenGLLoader loader;

};

QtGateway::QtGateway(bool const enableDebugOutput)
    : impl{std::make_unique<Impl>(enableDebugOutput)}
{
}

QtGateway::QtGateway(QtGateway && rhs) noexcept = default;

auto QtGateway::operator = (QtGateway && rhs) noexcept
    -> QtGateway & = default;

QtGateway::~QtGateway() = default;

auto QtGateway::createWindow()
    -> QtWindow *
{
    return impl->createWindow();
}

auto QtGateway::createWindow(QWidget & parent)
    -> QtWindow *
{
    return impl->createWindow(parent);
}

} // namespace ape::qt
