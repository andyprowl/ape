#pragma once

#include <Ape/World/Scene/Camera.hpp>

#include <Basix/Mathematics/Size.hpp>
#include <Basix/Signal/ScopedSignalConnection.hpp>

namespace ape
{

class SpotLight;

class SpotLightView
{

public:

    SpotLightView(SpotLight const & light, basix::Size<int> const & viewSize);
    
    SpotLightView(SpotLightView const & rhs) = delete;

    SpotLightView(SpotLightView && rhs) noexcept;

    auto operator = (SpotLightView const & rhs)
        -> SpotLightView & = delete;

    auto operator = (SpotLightView && rhs) noexcept
        -> SpotLightView &;

    ~SpotLightView() = default;

    auto getCamera() const
        -> const Camera &;

    auto setViewSize(basix::Size<int> const & newViewSize)
        -> void;

private:

    auto makeLightCamera() const
        -> Camera;

    auto computeFarDistance() const
        -> float;

    auto updateLightCamera()
        -> void;

    auto registerForLightChangeNotifications()
        -> basix::ScopedSignalConnection;

private:

    SpotLight const * light;

    basix::Size<int> viewSize;

    Camera camera;

    basix::ScopedSignalConnection onLightChangedConnection;

};

inline auto getTransformation(SpotLightView const & view)
    -> glm::mat4 const &
{
    return view.getCamera().getTransformation();
}

} // namespace ape
