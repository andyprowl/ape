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
    /**
     * Attenuation is a multiplier in the [0..1] range depending on distance from the light as per
     * the following formula:
     *
     *     a(d) = 1 / (q * d^2 + l * d + k)
     *
     * Where 'q' is the quadratic component, 'l' is the linear component and 'k' is the constant
     * component. **All of these components are non-negative**.
     *
     * We want to determine the farthest distance for which the light can give a significant color
     * contribution. A significant color contribution is any contribution higher than a minimum
     * threshold we define (say t).
     *
     * Our far distance is equal to the distance d such that a(d) = t. Therefore:
     *
     *     1 / (q * d^2 + l * d + k) = t
     *               ==>
     *     q * d^2 + l * d + k = (1 / t)
     *               ==>
     *     q * d^2 + l * d + k - (1 / t) = 0
     *
     * This is a quadratic equation of the form ax^2 + bx + c = 0, with:
     *
     *     a = q;
     *     b = l;
     *     c = k - 1 / t;
     *
     * In order to determine a meaningful minimum contribution, we recognize that color intensities
     * have 256 possible values, so the intensity quantum is given by (1.0 / 256).
     *
     * For reasons not completely understood, however, this quantum must be halved in order not to
     * have premature clipping in the fragment shader.
    **/

    constexpr auto const intensityQuantum = 0.5f / 256;

    constexpr auto const minimumContributionInverse = 1.0f / intensityQuantum;

    auto const & attenuation = light->getAttenuation();

    auto const a = attenuation.quadratic;

    auto const b = attenuation.linear;

    auto const c = attenuation.constant - minimumContributionInverse;

    auto const lightReach = (-b + glm::sqrt(b * b - 4.0f * a * c)) / (2.0f * a);

    return (glm::min(lightReach, 100.0f));
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
