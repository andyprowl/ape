#pragma once

namespace ape
{

class InputHandler
{

public:

    virtual ~InputHandler() = default;

    virtual auto processInput(double lastFrameDuration)
        -> void = 0;

};

} // namespace ape
