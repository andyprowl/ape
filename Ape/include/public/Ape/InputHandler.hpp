#pragma once

class InputHandler
{

public:

    virtual ~InputHandler() = default;

    virtual auto processInput(double lastFrameDuration)
        -> void = 0;

};
