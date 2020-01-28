#pragma once

#include <Ape/Rendering/Lighting/SpotLightView.hpp>

#include <Glow/Shader/Uniform.hpp>

namespace glow
{

template<>
class Uniform<ape::SpotLightView> : private Uniform<glm::mat4>
{

public:

    using ValueType = ape::SpotLightView;

public:

    using Uniform<glm::mat4>::Uniform;

    auto set(ValueType const & lightView)
    {
        constexpr auto const normalizationMatrix = glm::mat4{
            {0.5f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.5f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.5f, 0.0f},
            {0.5f, 0.5f, 0.5f, 1.0f}};

        Uniform<glm::mat4>::set(normalizationMatrix * lightView.getCamera().getTransformation());
    }

    auto operator = (ValueType const & lightView)
        -> Uniform &
    {
        set(lightView);

        return *this;
    }

};

} // namespace glow
