#pragma once

#include <Ape/Lighting/LightSystemView.hpp>

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

    UnsizedVectorUniform<Uniform<glm::mat4>> spot;

    UnsizedVectorUniform<Uniform<glm::mat4>> directional;

};

} // namespace glow
