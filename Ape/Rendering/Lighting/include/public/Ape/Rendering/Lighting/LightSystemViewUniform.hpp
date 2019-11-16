#pragma once

#include <Ape/Rendering/Lighting/DirectionalLightViewUniform.hpp>
#include <Ape/Rendering/Lighting/LightSystemView.hpp>
#include <Ape/Rendering/Lighting/SpotLightViewUniform.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/VectorUniform.hpp>

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

    auto set(ValueType const & lightSystem)
        -> void
    {
        spot = lightSystem.getSpotView();

        directional = lightSystem.getDirectionalView();
    }

    auto operator = (ValueType const & lightSystem)
        -> Uniform &
    {
        set(lightSystem);

        return *this;
    }

public:

    UnsizedVectorUniform<Uniform<ape::SpotLightView>> spot;

    UnsizedVectorUniform<Uniform<ape::DirectionalLightView>> directional;

};

} // namespace glow
