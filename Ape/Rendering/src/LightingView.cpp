#include <Ape/Rendering/LightingView.hpp>

#include <Ape/Scene/Camera.hpp>
#include <Ape/Scene/Lighting.hpp>

#include <Foundational/Range/Transform.hpp>

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

auto makePointLightView(Lighting const & lighting)
    -> std::vector<PointLightView>
{
    return transform(lighting.point, [] (PointLight const & light)
    {
        return makePointLightView(light);
    });
}

auto makeSpotLightView(SpotLight const & light, Size<int> const & viewSize)
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

auto makeSpotLightView(Lighting const & lighting, Size<int> const & viewSize)
    -> std::vector<glm::mat4>
{
    return transform(lighting.spot, [&viewSize] (SpotLight const & light)
    {
        return makeSpotLightView(light, viewSize);
    });
}

auto makeDirectionalLightView(DirectionalLight const & light)
    -> glm::mat4
{
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

auto makeDirectionalLightView(Lighting const & lighting)
    -> std::vector<glm::mat4>
{
    return transform(lighting.directional, [] (DirectionalLight const & light)
    {
        return makeDirectionalLightView(light);
    });
}

} // unnamed namespace

LightingView::LightingView(Lighting const & lighting, Size<int> const & viewSize)
    : lighting{&lighting}
    , viewSize{viewSize}
    , pointView{makePointLightView(lighting)}
    , spotView{makeSpotLightView(lighting, viewSize)}
    , directionalView{makeDirectionalLightView(lighting)}
    , pointLightChangeHandlerConnections{registerForPointLightChangeNotifications()}
    , spotLightChangeHandlerConnections{registerForSpotLightChangeNotifications()}
    , directionalLightChangeHandlerConnections{registerForDirectionalLightChangeNotifications()}
{
}

auto LightingView::getLighting() const
    -> Lighting const &
{
    return *lighting;
}

auto LightingView::getPointView() const
    -> std::vector<PointLightView> const &
{
    return pointView;
}

auto LightingView::getSpotView() const
    -> std::vector<glm::mat4> const &
{
    return spotView;
}

auto LightingView::getDirectionalView() const
    -> std::vector<glm::mat4> const &
{
    return directionalView;
}

auto LightingView::setViewSize(Size<int> const & newViewSize)
    -> void
{
    viewSize = newViewSize;

    for (auto i = 0u; i < spotView.size(); ++i)
    {
        spotView[i] = makeSpotLightView(lighting->spot[i], viewSize);
    }
}

auto LightingView::registerForPointLightChangeNotifications()
    -> std::vector<ScopedSignalConnection>
{
    return transform(lighting->point, [this] (PointLight const & light)
    {
        return registerForLightChangeNotifications(light);
    });
}

auto LightingView::registerForSpotLightChangeNotifications()
    -> std::vector<ScopedSignalConnection>
{
    return transform(lighting->spot, [this] (SpotLight const & light)
    {
        return registerForLightChangeNotifications(light);
    });
}

auto LightingView::registerForDirectionalLightChangeNotifications()
    -> std::vector<ScopedSignalConnection>
{
    return transform(lighting->directional, [this] (DirectionalLight const & light)
    {
        return registerForLightChangeNotifications(light);
    });
}

template<typename LightType>
auto LightingView::registerForLightChangeNotifications(LightType const & light)
    -> ScopedSignalConnection
{
    return light.onLightChanged.registerHandler([this, &light]
    {
        udpateLightView(light);
    });
}

auto LightingView::udpateLightView(PointLight const & light)
    -> void
{
    auto const index = std::distance(lighting->point.data(), &light);

    pointView[index] = makePointLightView(light);
}

auto LightingView::udpateLightView(SpotLight const & light)
    -> void
{
    auto const index = std::distance(lighting->spot.data(), &light);

    spotView[index] = makeSpotLightView(light, viewSize);
}

auto LightingView::udpateLightView(DirectionalLight const & light)
    -> void
{
    auto const index = std::distance(lighting->directional.data(), &light);

    directionalView[index] = makeDirectionalLightView(light);
}

} // namespace ape
