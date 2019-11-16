#include <Ape/Rendering/Lighting/DirectionalLightView.hpp>

#include <Ape/World/Scene/DirectionalLight.hpp>

namespace ape
{

DirectionalLightView::DirectionalLightView(DirectionalLight const & light)
    : light{&light}
    , camera{makeLightCamera()}
    , onLightChangedConnection{registerForLightChangeNotifications()}
{
}

DirectionalLightView::DirectionalLightView(DirectionalLightView && rhs) noexcept
    : light{rhs.light}
    , camera{std::move(rhs.camera)}
    , onLightChangedConnection{registerForLightChangeNotifications()}
{
}

auto DirectionalLightView::operator = (DirectionalLightView && rhs) noexcept
    -> DirectionalLightView &
{
    light = rhs.light;
    
    camera = std::move(rhs.camera);
    
    onLightChangedConnection = registerForLightChangeNotifications();

    return *this;
}

auto DirectionalLightView::getCamera() const
    -> const Camera &
{
    return camera;
}

auto DirectionalLightView::makeLightCamera() const
    -> Camera
{
    // TODO: This matrix calculation is incorrect and hacky an needs to be changed.
    // Cascaded shadow maps should be used.

    auto const nearPlane = 0.1f;

    auto const farPlane = 100.0f;

    auto const direction = light->getDirection();

    auto const source = -(direction * farPlane / 2.0f);

    auto const viewUp = glm::vec3{0.0f, 1.0f, 0.0f};

    return {
        CameraView::Placement{source, direction, viewUp},
        OrthographicProjection::Frustum{-30.0f, +30.0f, -30.0f, +30.0f, nearPlane, farPlane}};
}

auto DirectionalLightView::updateLightCamera()
    -> void
{
    camera = makeLightCamera();
}

auto DirectionalLightView::registerForLightChangeNotifications()
    -> basix::ScopedSignalConnection
{
    return light->onLightChanged.registerHandler([this]
    {
        updateLightCamera();
    });
}

} // namespace ape
