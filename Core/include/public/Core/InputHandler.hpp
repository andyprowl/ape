#pragma once

#include <chrono>

namespace ape
{

class InputHandler
{

public:

    virtual ~InputHandler() = default;

    virtual auto onFrame(std::chrono::nanoseconds frameDuration)
        -> void = 0;

};

} // namespace ape
