#include <Ape/BlinnPhongKeySwitcher.hpp>

#include <Ape/StandardShaderProgram.hpp>
#include <Ape/Window.hpp>

namespace ape
{

BlinnPhongKeySwitcher::BlinnPhongKeySwitcher(Window const & window, StandardShaderProgram & shader)
    : shader{&shader}
    , onKeyPressedConnection{registerKeyPressedHandler(window)}
{
}

auto BlinnPhongKeySwitcher::toggleBlinnPhongModel() const
    -> void
{
    shader->useBlinnPhongModel = !shader->useBlinnPhongModel;
}

auto BlinnPhongKeySwitcher::registerKeyPressedHandler(Window const & window) const
    -> ScopedSignalConnection
{
    return onKeyPressed(window, Key::keyB, [this] (KeyModifier const)
    {
        toggleBlinnPhongModel();
    });
}

} // namespace ape
