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
        //: point{program, prefix + ".point"}
        : spot{program, prefix + ".spot"}
        , directional{program, prefix + ".directional"}
    {
    }

    auto set(ValueType const & lighting)
        -> void
    {
        //point = lighting.getPointView();

        spot = lighting.getSpotView();

        directional = lighting.getDirectionalView();
    }

    auto operator = (ValueType const & light)
        -> Uniform &
    {
        set(light);

        return *this;
    }

public:

    //UnsizedVectorUniform<Uniform<glm::mat4>> point;

    UnsizedVectorUniform<Uniform<glm::mat4>> spot;

    UnsizedVectorUniform<Uniform<glm::mat4>> directional;

};

} // namespace ape
