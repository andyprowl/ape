#include <Rendering/LightingView.hpp>

#include <Scene/Camera.hpp>
#include <Scene/Lighting.hpp>

#include <Range/Transform.hpp>

namespace ape
{

namespace
{

auto makeSpotView(SpotLight const & light, Size<int> const & viewSize)
    -> glm::mat4
{
    auto const aspectRatio = viewSize.width / static_cast<float>(viewSize.height);

    auto const fieldOfView = light.getCutoff().outer * 2.0f;

    auto const viewUp = glm::vec3{0.0f, 1.0f, 0.0f};
    
    auto const camera = Camera{
        CameraView::Placement{light.getPosition(), light.getDirection(), viewUp},
        PerspectiveProjection::Frustum{fieldOfView, aspectRatio, 0.1f, 100.0f}};

    return camera.getTransformation();
}

auto makeSpotView(Lighting const & lighting, Size<int> const & viewSize)
    -> std::vector<glm::mat4>
{
    return transform(lighting.spot, [&viewSize] (SpotLight const & light)
    {
        return makeSpotView(light, viewSize);
    });
}

auto makeDirectionalView(DirectionalLight const & light)
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

auto makeDirectionalView(Lighting const & lighting)
    -> std::vector<glm::mat4>
{
    return transform(lighting.directional, [] (DirectionalLight const & light)
    {
        return makeDirectionalView(light);
    });
}

} // unnamed namespace

LightingView::LightingView(Lighting const & lighting, Size<int> const & viewSize)
    : lighting{&lighting}
    , viewSize{viewSize}
    , spotView{makeSpotView(lighting, viewSize)}
    , directionalView{makeDirectionalView(lighting)}
    , spotLightChangeHandlerConnections{registerForSpotLightChangeNotifications()}
    , directionalLightChangeHandlerConnections{registerForDirectionalLightChangeNotifications()}
{
}

auto LightingView::getDirectionalView() const
    -> std::vector<glm::mat4> const &
{
    return directionalView;
}

auto LightingView::getSpotView() const
    -> std::vector<glm::mat4> const &
{
    return spotView;
}

auto LightingView::getPointView() const
    -> std::vector<glm::mat4> const &
{
    return pointView;
}

auto LightingView::setViewSize(Size<int> const & newViewSize)
    -> void
{
    viewSize = newViewSize;

    for (auto i = 0u; i < spotView.size(); ++i)
    {
        spotView[i] = makeSpotView(lighting->spot[i], viewSize);
    }
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

auto LightingView::udpateLightView(SpotLight const & light)
    -> void
{
    auto const index = std::distance(lighting->spot.data(), &light);

    spotView[index] = makeSpotView(light, viewSize);
}

auto LightingView::udpateLightView(DirectionalLight const & light)
    -> void
{
    auto const index = std::distance(lighting->directional.data(), &light);

    directionalView[index] = makeDirectionalView(light);
}

} // namespace ape
