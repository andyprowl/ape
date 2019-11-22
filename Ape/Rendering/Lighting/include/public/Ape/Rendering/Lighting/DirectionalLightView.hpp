#pragma once

#include <Ape/World/Scene/Camera.hpp>

#include <Basix/Mathematics/Size.hpp>
#include <Basix/Signal/ScopedSignalConnection.hpp>

namespace ape
{

class DirectionalLight;

class DirectionalLightView
{

public:

    explicit DirectionalLightView(DirectionalLight const & light);
    
    DirectionalLightView(DirectionalLightView const & rhs) = delete;

    DirectionalLightView(DirectionalLightView && rhs) noexcept;

    auto operator = (DirectionalLightView const & rhs)
        -> DirectionalLightView & = delete;

    auto operator = (DirectionalLightView && rhs) noexcept
        -> DirectionalLightView &;

    ~DirectionalLightView() = default;

    auto getCamera() const
        -> const Camera &;

private:

    auto makeLightCamera() const
        -> Camera;

    auto updateLightCamera()
        -> void;

    auto registerForLightChangeNotifications()
        -> basix::ScopedSignalConnection;

private:

    DirectionalLight const * light;

    Camera camera;

    basix::ScopedSignalConnection onLightChangedConnection;

};

inline auto getTransformation(DirectionalLightView const & view)
    -> glm::mat4 const &
{
    return view.getCamera().getTransformation();
}

} // namespace ape
