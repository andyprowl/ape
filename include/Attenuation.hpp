#pragma once

class Attenuation
{

public:

    Attenuation(float const constant, float const linear, float const quadratic)
        : constant{constant}
        , linear{linear}
        , quadratic{quadratic}
    {
    }

public:

    float constant;

    float linear;

    float quadratic;

};
