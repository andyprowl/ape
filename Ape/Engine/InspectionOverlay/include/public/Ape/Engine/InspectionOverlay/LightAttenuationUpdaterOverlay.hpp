#pragma once

namespace ape
{

class Attenuation;

template<typename LightType>
class LightAttenuationUpdaterOverlay
{

public:

    auto updateLightAttenuation(LightType & light)
        -> void;

private:

    auto updateConstantComponent(Attenuation & attenuation)
        -> bool;

    auto updateQuadraticComponent(Attenuation & attenuation)
        -> bool;

};

} // namespace ape
