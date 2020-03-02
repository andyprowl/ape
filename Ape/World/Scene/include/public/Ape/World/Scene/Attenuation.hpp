#pragma once

namespace ape
{

class Attenuation
{

public:

    Attenuation(float const constant, float const quadratic)
        : constant{constant}
        , quadratic{quadratic}
    {
    }

public:

    float constant;

    float quadratic;

};

auto computeAttenuationDistance(Attenuation const & attenuation)
    -> float;

} // namespace ape
