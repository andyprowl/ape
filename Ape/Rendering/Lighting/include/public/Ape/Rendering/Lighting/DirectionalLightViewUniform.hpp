#pragma once

#include <Ape/Rendering/Lighting/DirectionalLightView.hpp>

#include <Glow/Shader/Uniform.hpp>

namespace glow
{

template<>
class Uniform<ape::DirectionalLightView> : private Uniform<glm::mat4>                               
{

public:

    using ValueType = ape::DirectionalLightView;

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

};

} // namespace glow
