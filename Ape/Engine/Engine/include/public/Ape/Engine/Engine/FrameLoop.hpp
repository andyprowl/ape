#pragma once

#include <functional>

namespace ape
{

class FrameLoop
{

public:

    using FrameProcessor = std::function<auto () -> void>;

public:

    virtual ~FrameLoop() = default;

    virtual auto start(FrameProcessor processor)
        -> void = 0;

    virtual auto stop()
        -> void = 0;

};

} // namespace ape
