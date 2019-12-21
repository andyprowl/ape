#pragma once

#include <Basix/Signal/Signal.hpp>

namespace ape
{

class CameraSelector;
class Scene;
class Viewport;
class TaskTimeProfiler;

class Renderer
{

public:

    virtual ~Renderer() = default;

    virtual auto render()
        -> void = 0;

    virtual auto getViewport() const
        -> Viewport = 0;

    virtual auto setViewport(Viewport const & viewport)
        -> void = 0;
    
    virtual auto getCameraSelector() const
        -> CameraSelector const & = 0;

    virtual auto setCameraSelector(CameraSelector const & newSelector)
        -> void = 0;

    virtual auto setProfiler(TaskTimeProfiler & profiler)
        -> void = 0;

};

} // namespace ape
