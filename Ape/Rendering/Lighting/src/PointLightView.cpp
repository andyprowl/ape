#include <Ape/Rendering/Lighting/PointLightView.hpp>

#include <Ape/World/Scene/PointLight.hpp>

namespace ape
{

namespace
{

auto makePointLightDirectionView(
    glm::vec3 const & position,
    glm::vec3 const & direction,
    glm::vec3 const & viewUp)
    -> Camera
{
    // TODO: Is it correct not to compute the aspect ratio from the view's aspect ratio?
    // (like we do for spot lights)
    auto const aspectRatio = 1.0;

    auto const fieldOfView = glm::radians(90.0f);

    auto const nearPlaneDistance = 0.1f;
    
    auto const farPlaneDistance = 100.0f;

    return {
        CameraView::Placement{position, direction, viewUp},
        PerspectiveProjection::Frustum{
            fieldOfView,
            aspectRatio,
            nearPlaneDistance,
            farPlaneDistance}};
}

} // unnamed namespace

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

    // The choice of the "up" vectors is due to the OpenGL's convention for cube maps.
    // See https://stackoverflow.com/a/11694336/1932150 for a convenient diagram.

    return {
        makePointLightDirectionView(position, {+1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}),
        makePointLightDirectionView(position, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}),
        makePointLightDirectionView(position, {0.0f, +1.0f, 0.0f}, {0.0f, 0.0f, +1.0f}),
        makePointLightDirectionView(position, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}),
        makePointLightDirectionView(position, {0.0f, 0.0f, +1.0f}, {0.0f, -1.0f, 0.0f}),
        makePointLightDirectionView(position, {0.0f, 0.0f, -1.0f}, {0.0f, -1.0f, 0.0f})};
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
