#pragma once

#include <Ape/Rendering/LightSystemView.hpp>

#include <Ape/GpuResource/ShaderProgram.hpp>
#include <Ape/GpuResource/VectorUniform.hpp>

#include <vector>

namespace ape
{

template<>
class Uniform<LightSystemView>
{

public:

    using ValueType = LightSystemView;

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

} // namespace ape
