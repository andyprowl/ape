#pragma once

#include <Ape/Rendering/LightingView.hpp>

#include <Ape/GpuResource/ShaderProgram.hpp>
#include <Ape/GpuResource/VectorUniform.hpp>

#include <vector>

namespace ape
{

template<>
class Uniform<LightingView>
{

public:

    using ValueType = LightingView;

public:

    Uniform(ShaderProgram & program, std::string const & prefix)
        : spot{program, prefix + ".spot"}
        , directional{program, prefix + ".directional"}
    {
    }

    auto set(ValueType const & lighting)
        -> void
    {
        spot = lighting.getSpotView();

        directional = lighting.getDirectionalView();
    }

    auto operator = (ValueType const & lighting)
        -> Uniform &
    {
        set(lighting);

        return *this;
    }

public:

    UnsizedVectorUniform<Uniform<glm::mat4>> spot;

    UnsizedVectorUniform<Uniform<glm::mat4>> directional;

};

} // namespace ape
