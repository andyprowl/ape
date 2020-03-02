#include <Ape/Rendering/Lighting/SpotLightView.hpp>

#include <Ape/World/Scene/SpotLight.hpp>

namespace ape
{

SpotLightView::SpotLightView(SpotLight const & light, basix::Size2d<int> const & viewSize)
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

auto SpotLightView::setViewSize(basix::Size2d<int> const & newViewSize)
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

    // Correct setting of far distance is important, because the shader will not compute lighting
    // for fragments that are outside the light's frustum (in this case, there are beyond the far
    // plane).
    auto const farDistance = computeFarDistance();

    return {
        CameraView::System{light->getPosition(), light->getDirection(), viewUp},
        PerspectiveProjection::Frustum{fieldOfView, aspectRatio, 0.1f, farDistance}};
}

auto SpotLightView::computeFarDistance() const
    -> float
{
    auto const & attenuation = light->getAttenuation();

    return computeAttenuationDistance(attenuation);
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
