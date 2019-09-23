#include <Ape/Window.hpp>

namespace ape
{

auto onKeyPressed(
    Window const & window,
    std::function<auto (Key key, KeyModifier modifier) -> void> handler)
    -> ScopedSignalConnection
{
    return window.onKeyboard.registerHandler(
        [handler = std::move(handler)] (
            Key const key,
            KeyAction const action,
            KeyModifier const modifier)
    {
        if (action == KeyAction::press)
        {
            return handler(key, modifier);
        }
    });
}

auto onKeyPressed(
    Window const & window,
    Key const key,
    std::function<auto (KeyModifier modifier) -> void> handler)
    -> ScopedSignalConnection
{
    return window.onKeyboard.registerHandler(
        [key, handler = std::move(handler)] (
            Key const k,
            KeyAction const action,
            KeyModifier const modifier)
    {
        if ((action == KeyAction::press) && (k == key))
        {
            return handler(modifier);
        }
    });
}

} // namespace ape
