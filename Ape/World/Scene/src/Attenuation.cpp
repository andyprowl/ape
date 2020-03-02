#include <Ape/World/Scene/Attenuation.hpp>

#include <glm/geometric.hpp>

namespace ape
{

auto computeAttenuationDistance(Attenuation const & attenuation)
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
     *     1 / (q * d^2 + k) = t
     *             ==>
     *     q * d^2 + k = (1 / t)
     *             ==>
     *     q * d^2 + k - (1 / t) = 0
     *
     * This is a quadratic equation of the form ax^2 + bx + c = 0, with:
     *
     *     a = q;
     *     b = 0;
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

    auto const a = attenuation.quadratic;

    auto const c = attenuation.constant - minimumContributionInverse;

    auto const lightReach = (glm::sqrt(-4.0f * a * c)) / (2.0f * a);

    return (glm::min(lightReach, 100.0f));
}

} // namespace ape
