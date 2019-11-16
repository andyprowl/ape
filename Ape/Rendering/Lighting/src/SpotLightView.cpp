#include <Ape/Rendering/Lighting/SpotLightView.hpp>

#include <Ape/World/Scene/SpotLight.hpp>

namespace ape
{

SpotLightView::SpotLightView(SpotLight const & light, basix::Size<int> const & viewSize)
    : light{&light}
    , viewSize{viewSize}
    , camera{makeLightCamera()}
    , onLightChangedConnection{registerForLightChangeNotifications()}
{
}

SpotLightView::SpotLightView(SpotLightView && rhs) noexcept
    : light{rhs.light}
    , viewSize{rhs.viewSize}
    , camera{std::move(rhs.camera)}
    , onLightChangedConnection{registerForLightChangeNotifications()}
{
}

auto SpotLightView::operator = (SpotLightView && rhs) noexcept
    -> SpotLightView &
{
    light = rhs.light;
    
    viewSize = rhs.viewSize;
    
    camera = std::move(rhs.camera);
    
    onLightChangedConnection = registerForLightChangeNotifications();

    return *this;
}

auto SpotLightView::getCamera() const
    -> const Camera &
{
    return camera;
}

auto SpotLightView::setViewSize(basix::Size<int> const & newViewSize)
    -> void
{
    viewSize = newViewSize;

    updateLightCamera();
}

auto SpotLightView::makeLightCamera() const
    -> Camera
{
    // TODO: Is it correct to compute the aspect ratio from the view's aspect ratio?
    // (unlike for point lights where we use a fixed aspect ratio of 1.0)
    auto const aspectRatio = viewSize.width / static_cast<float>(viewSize.height);

    auto const fieldOfView = light->getCutoff().outer * 2.0f;

    auto const viewUp = glm::vec3{0.0f, 1.0f, 0.0f};
    
    return {
        CameraView::Placement{light->getPosition(), light->getDirection(), viewUp},
        PerspectiveProjection::Frustum{fieldOfView, aspectRatio, 0.1f, 100.0f}};
}

auto SpotLightView::updateLightCamera()
    -> void
{
    camera = makeLightCamera();
}

auto SpotLightView::registerForLightChangeNotifications()
    -> basix::ScopedSignalConnection
{
    return light->onLightChanged.registerHandler([this]
    {
        updateLightCamera();
    });
}

} // namespace ape
