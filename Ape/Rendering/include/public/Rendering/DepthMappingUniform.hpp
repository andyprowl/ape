#pragma once

#include <Rendering/DepthMapping.hpp>

#include <GpuResource/ScopedBinder.hpp>
#include <GpuResource/ShaderProgram.hpp>
#include <GpuResource/TextureUniform.hpp>
#include <GpuResource/VectorUniform.hpp>

#include <vector>

namespace ape
{

template<>
class Uniform<DepthMapping>
{

public:

    using ValueType = DepthMapping;

public:

    Uniform(
        ShaderProgram & program,
        std::string const & prefix,
        int const firstDepthMapUnit)
        //: point{program, prefix + ".point"}
        : spot{program, prefix + ".spot"}
        , directional{program, prefix + ".directional"}
        , maxNumOfLightsPerType{8}
        , firstDepthMapUnit{firstDepthMapUnit}
    {
        bindSamplers(program);
    }

    auto set(DepthMapping const & mapping)
        -> void
    {
        //setPointMapping(mapping);

        setSpotMapping(mapping);

        setDirectionalMapping(mapping);
    }
    
    auto operator = (ValueType const & mapping)
        -> Uniform &
    {
        set(mapping);

        return *this;
    }

public:

    //UnsizedVectorUniform<Uniform<Texture>> point;

    UnsizedVectorUniform<Uniform<Texture>> spot;

    UnsizedVectorUniform<Uniform<Texture>> directional;

private:

    auto bindSamplers(ShaderProgram & program)
        -> void
    {
        auto const shaderBinder = ScopedBinder{program};

        auto index = firstDepthMapUnit;

        /*
        for (auto i = 0; i < maxNumOfLightsPerType; ++i)
        {
            point[i].bind(index++);
        }
        */

        for (auto i = 0; i < maxNumOfLightsPerType; ++i)
        {
            spot[i].bind(index++);
        }

        for (auto i = 0; i < maxNumOfLightsPerType; ++i)
        {
            directional[i].bind(index++);
        }
    }

    /*
    auto setPointMapping(DepthMapping const & mapping)
        -> void
    {
        setTextures(mapping.getPointMapping(), point);
    }
    */

    auto setSpotMapping(DepthMapping const & mapping)
        -> void
    {
        setTextures(mapping.getSpotMapping(), spot);
    }

    auto setDirectionalMapping(DepthMapping const & mapping)
        -> void
    {
        setTextures(mapping.getDirectionalMapping(), directional);
    }

    auto setTextures(
        std::vector<DepthMap> const & source,
        UnsizedVectorUniform<Uniform<Texture>> & target)
        -> void
    {
        for (auto i = 0; i < static_cast<int>(source.size()); ++i)
        {
            target[i] = source[i].getTexture();
        }
    }

private:

    int maxNumOfLightsPerType;

    int firstDepthMapUnit;

};

} // namespace ape
