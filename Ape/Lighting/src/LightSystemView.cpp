#include <Ape/Lighting/LightSystemView.hpp>

#include <Ape/Scene/Camera.hpp>
#include <Ape/Scene/LightSystem.hpp>

#include <Basix/Range/Transform.hpp>

namespace ape
{

namespace
{

auto makePointLightDirectionView(
    glm::vec3 const & position,
    glm::vec3 const & direction,
    glm::vec3 const & viewUp)
    -> glm::mat4
{
    // TODO: Is it correct not to compute the aspect ratio from the view's aspect ratio?
    // (like we do for spot lights)
    auto const aspectRatio = 1.0;

    auto const fieldOfView = glm::radians(90.0f);

    auto const nearPlaneDistance = 0.1f;
    
    auto const farPlaneDistance = 100.0f;

    auto const camera = Camera{
        CameraView::Placement{position, direction, viewUp},
        PerspectiveProjection::Frustum{
            fieldOfView,
            aspectRatio,
            nearPlaneDistance,
            farPlaneDistance}};

    return camera.getTransformation();
}

auto makePointLightView(PointLight const & light)
    -> PointLightView
{
    auto const & position = light.getPosition();

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

auto makePointLightView(LightSystem const & lightSystem)
    -> std::vector<PointLightView>
{
    return basix::transform(lightSystem.point, [] (PointLight const & light)
    {
        return makePointLightView(light);
    });
}

auto makeSpotLightView(SpotLight const & light, basix::Size<int> const & viewSize)
    -> glm::mat4
{
    // TODO: Is it correct to compute the aspect ratio from the view's aspect ratio?
    // (unlike for point lights where we use a fixed aspect ratio of 1.0)
    auto const aspectRatio = viewSize.width / static_cast<float>(viewSize.height);

    auto const fieldOfView = light.getCutoff().outer * 2.0f;

    auto const viewUp = glm::vec3{0.0f, 1.0f, 0.0f};
    
    auto const camera = Camera{
        CameraView::Placement{light.getPosition(), light.getDirection(), viewUp},
        PerspectiveProjection::Frustum{fieldOfView, aspectRatio, 0.1f, 100.0f}};

    return camera.getTransformation();
}

auto makeSpotLightView(LightSystem const & lightSystem, basix::Size<int> const & viewSize)
    -> std::vector<glm::mat4>
{
    return basix::transform(lightSystem.spot, [&viewSize] (SpotLight const & light)
    {
        return makeSpotLightView(light, viewSize);
    });
}

auto makeDirectionalLightView(DirectionalLight const & light)
    -> glm::mat4
{
    // TODO: This matrix calculation is incorrect and hacky an needs to be changed.
    // Cascaded shadow maps should be used.

    auto const nearPlane = 0.1f;

    auto const farPlane = 100.0f;

    auto const direction = light.getDirection();

    auto const source = -(direction * farPlane / 2.0f);

    auto const viewUp = glm::vec3{0.0f, 1.0f, 0.0f};

    auto const camera = Camera{
        CameraView::Placement{source, direction, viewUp},
        OrthographicProjection::Frustum{-30.0f, +30.0f, -30.0f, +30.0f, nearPlane, farPlane}};

    return camera.getTransformation();
}

auto makeDirectionalLightView(LightSystem const & lightSystem)
    -> std::vector<glm::mat4>
{
    return basix::transform(lightSystem.directional, [] (DirectionalLight const & light)
    {
        return makeDirectionalLightView(light);
    });
}

} // unnamed namespace

LightSystemView::LightSystemView(
    LightSystem const & lightSystem,
    basix::Size<int> const & viewSize)
    : lightSystem{&lightSystem}
    , viewSize{viewSize}
    , pointView{makePointLightView(lightSystem)}
    , spotView{makeSpotLightView(lightSystem, viewSize)}
    , directionalView{makeDirectionalLightView(lightSystem)}
    , pointLightChangeHandlerConnections{registerForPointLightChangeNotifications()}
    , spotLightChangeHandlerConnections{registerForSpotLightChangeNotifications()}
    , directionalLightChangeHandlerConnections{registerForDirectionalLightChangeNotifications()}
{
}

auto LightSystemView::getLighting() const
    -> LightSystem const &
{
    return *lightSystem;
}

auto LightSystemView::getPointView() const
    -> std::vector<PointLightView> const &
{
    return pointView;
}

auto LightSystemView::getSpotView() const
    -> std::vector<glm::mat4> const &
{
    return spotView;
}

auto LightSystemView::getDirectionalView() const
    -> std::vector<glm::mat4> const &
{
    return directionalView;
}

auto LightSystemView::setViewSize(basix::Size<int> const & newViewSize)
    -> void
{
    viewSize = newViewSize;

    for (auto i = 0u; i < spotView.size(); ++i)
    {
        spotView[i] = makeSpotLightView(lightSystem->spot[i], viewSize);
    }
}

auto LightSystemView::registerForPointLightChangeNotifications()
    -> std::vector<basix::ScopedSignalConnection>
{
    return basix::transform(lightSystem->point, [this] (PointLight const & light)
    {
        return registerForLightChangeNotifications(light);
    });
}

auto LightSystemView::registerForSpotLightChangeNotifications()
    -> std::vector<basix::ScopedSignalConnection>
{
    return basix::transform(lightSystem->spot, [this] (SpotLight const & light)
    {
        return registerForLightChangeNotifications(light);
    });
}

auto LightSystemView::registerForDirectionalLightChangeNotifications()
    -> std::vector<basix::ScopedSignalConnection>
{
    return basix::transform(lightSystem->directional, [this] (DirectionalLight const & light)
    {
        return registerForLightChangeNotifications(light);
    });
}

template<typename LightType>
auto LightSystemView::registerForLightChangeNotifications(LightType const & light)
    -> basix::ScopedSignalConnection
{
    return light.onLightChanged.registerHandler([this, &light]
    {
        udpateLightView(light);
    });
}

auto LightSystemView::udpateLightView(PointLight const & light)
    -> void
{
    auto const index = std::distance(lightSystem->point.data(), &light);

    pointView[index] = makePointLightView(light);
}

auto LightSystemView::udpateLightView(SpotLight const & light)
    -> void
{
    auto const index = std::distance(lightSystem->spot.data(), &light);

    spotView[index] = makeSpotLightView(light, viewSize);
}

auto LightSystemView::udpateLightView(DirectionalLight const & light)
    -> void
{
    auto const index = std::distance(lightSystem->directional.data(), &light);

    directionalView[index] = makeDirectionalLightView(light);
}

} // namespace ape
