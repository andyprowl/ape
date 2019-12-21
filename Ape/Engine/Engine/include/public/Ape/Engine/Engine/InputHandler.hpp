#pragma once

#include <chrono>

namespace ape
{

class Engine;

class InputHandler
{

public:

    virtual ~InputHandler() = default;

    virtual auto onFrame(std::chrono::nanoseconds frameDuration)
        -> void = 0;

    virtual auto getEngine() const
        -> Engine & = 0;

    virtual auto setEngine(Engine & engine)
        -> void = 0;

};

} // namespace ape
