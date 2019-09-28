#pragma once

#include <Core/Keyboard.hpp>
#include <Core/Offset.hpp>
#include <Core/Position.hpp>

#include <chrono>

namespace ape
{

class InputHandler
{

public:

    virtual ~InputHandler() = default;

    virtual auto onFrame(std::chrono::nanoseconds frameDuration)
        -> void = 0;

    virtual auto onKeyPress(Key key, KeyModifier modifier)
        -> void = 0;

    virtual auto onKeyRelease(Key key, KeyModifier modifier)
        -> void = 0;

    virtual auto onMouseMove(Position<int> position)
        -> void = 0;

    virtual auto onMouseWheel(Offset<int> offset)
        -> void = 0;

    virtual auto onFocusAcquired()
        -> void = 0;

    virtual auto onFocusLost()
        -> void = 0;
};

} // namespace ape
