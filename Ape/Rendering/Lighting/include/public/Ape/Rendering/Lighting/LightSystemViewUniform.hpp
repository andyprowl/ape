#pragma once

#include <Ape/Rendering/Lighting/DirectionalLightViewUniform.hpp>
#include <Ape/Rendering/Lighting/LightSystemView.hpp>
#include <Ape/Rendering/Lighting/SpotLightViewUniform.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/UniformArray.hpp>

#include <vector>

namespace glow
{

template<>
class Uniform<ape::LightSystemView>
{

public:

    using ValueType = ape::LightSystemView;

public:

    Uniform(ShaderProgram & program, std::string const & prefix)
        : spot{program, prefix + ".spot"}
        , directional{program, prefix + ".directional"}
    {
    }

    auto set(ValueType const & view)
        -> void
    {
        spot.set(view.getSpotView());

        directional.set(view.getDirectionalView());
    }

public:

    UniformArray<Uniform<ape::SpotLightView>> spot;

    UniformArray<Uniform<ape::DirectionalLightView>> directional;

};

} // namespace glow
