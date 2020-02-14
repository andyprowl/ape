#pragma once

#include <Basix/Signal/Signal.hpp>

namespace ape
{

class CameraSelector;
class Scene;
class TaskTimeProfiler;
class Viewport;
class Window;

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
        -> CameraSelector & = 0;

    virtual auto setCameraSelector(CameraSelector & newSelector)
        -> void = 0;

    virtual auto getTargetSurface()
        -> Window & = 0;

    virtual auto setProfiler(TaskTimeProfiler & profiler)
        -> void = 0;

};

} // namespace ape
