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
        Uniform<glm::mat4>::set(lightView.getCamera().getTransformation());
    }

    auto operator = (ValueType const & lightView)
        -> Uniform &
    {
        set(lightView);

        return *this;
    }

};

} // namespace glow
