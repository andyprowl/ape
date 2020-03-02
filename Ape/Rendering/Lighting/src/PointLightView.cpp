#include <Ape/Rendering/Lighting/PointLightView.hpp>

#include <Ape/World/Scene/PointLight.hpp>

namespace ape
{

PointLightView::PointLightView(PointLight const & light)
    : light{&light}
    , faceCameras{makeLightFaceCameras()}
    , onLightChangedConnection{registerForLightChangeNotifications()}
{
}

PointLightView::PointLightView(PointLightView && rhs) noexcept
    : light{rhs.light}
    , faceCameras{makeLightFaceCameras()}
    , onLightChangedConnection{registerForLightChangeNotifications()}
{
}

auto PointLightView::operator = (PointLightView && rhs) noexcept
    -> PointLightView &
{
    light = rhs.light;
    
    faceCameras = std::move(rhs.faceCameras);
    
    onLightChangedConnection = registerForLightChangeNotifications();

    return *this;
}

auto PointLightView::getFaceCameras() const
    -> const FaceCameraSet &
{
    return faceCameras;
}

auto PointLightView::makeLightFaceCameras() const
    -> FaceCameraSet
{
    auto const & position = light->getPosition();

    auto const farDistance = computeCameraFarDistance();

    // The choice of the "up" vectors is due to the OpenGL's convention for cube maps.
    // See https://stackoverflow.com/a/11694336/1932150 for a convenient diagram.

    return {
        makeLightFaceCamera({position, {+1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, farDistance),
        makeLightFaceCamera({position, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, farDistance),
        makeLightFaceCamera({position, {0.0f, +1.0f, 0.0f}, {0.0f, 0.0f, +1.0f}}, farDistance),
        makeLightFaceCamera({position, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, farDistance),
        makeLightFaceCamera({position, {0.0f, 0.0f, +1.0f}, {0.0f, -1.0f, 0.0f}}, farDistance),
        makeLightFaceCamera({position, {0.0f, 0.0f, -1.0f}, {0.0f, -1.0f, 0.0f}}, farDistance)};
}

auto PointLightView::makeLightFaceCamera(
    CameraView::System const & system,
    float const farPlaneDistance) const
    -> Camera
{
    // TODO: Is it correct not to compute the aspect ratio from the depth map's aspect ratio?
    // (like we do for spot lights)
    constexpr auto const aspectRatio = 1.0f;

    constexpr auto const fieldOfView = glm::radians(90.0f);

    constexpr auto const nearPlaneDistance = 0.1f;

    auto const frustum = PerspectiveProjection::Frustum{
        fieldOfView,
        aspectRatio,
        nearPlaneDistance,
        farPlaneDistance};

    return {system, frustum};
}

auto PointLightView::computeCameraFarDistance() const
    -> float
{
    auto const & attenuation = light->getAttenuation();

    return ape::computeAttenuationDistance(attenuation);
}

auto PointLightView::updateLightFaceCameras()
    -> void
{
    faceCameras = makeLightFaceCameras();
}

auto PointLightView::registerForLightChangeNotifications()
    -> basix::ScopedSignalConnection
{
    return light->onLightChanged.registerHandler([this]
    {
        updateLightFaceCameras();
    });
}

} // namespace ape
