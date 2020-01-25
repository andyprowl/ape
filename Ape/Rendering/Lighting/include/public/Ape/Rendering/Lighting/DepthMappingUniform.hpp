#pragma once

#include <Ape/Rendering/Lighting/DepthMapping.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Shader/CubeTextureUniform.hpp>
#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/TextureUniform.hpp>
#include <Glow/Shader/VectorUniform.hpp>

#include <string>
#include <vector>

namespace glow
{

template<>
class Uniform<ape::DepthMapping>
{

public:

    using ValueType = ape::DepthMapping;

public:

    Uniform(
        ShaderProgram & program,
        std::string prefix,
        int const firstDepthMapUnit)
        : point{program, prefix + ".point"}
        , spot{program, prefix + ".spot"}
        , directional{program, std::move(prefix) + ".directional"}
        , maxNumOfPointLights{15}
        , maxNumOfSpotLights{8}
        , maxNumOfDirectionalLights{4}
        , firstDepthMapUnit{firstDepthMapUnit}
    {
        bindSamplers(program);
    }

    auto set(ape::DepthMapping const & mapping)
        -> void
    {
        setPointMapping(mapping);

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

    UnsizedVectorUniform<Uniform<CubeTexture>> point;

    UnsizedVectorUniform<Uniform<Texture>> spot;

    UnsizedVectorUniform<Uniform<Texture>> directional;

private:

    auto bindSamplers(ShaderProgram & program)
        -> void
    {
        auto const shaderBinder = bind(program);

        auto index = firstDepthMapUnit;

        for (auto i = 0; i < maxNumOfPointLights; ++i)
        {
            point[i].bind(index++);
        }

        for (auto i = 0; i < maxNumOfSpotLights; ++i)
        {
            spot[i].bind(index++);
        }

        for (auto i = 0; i < maxNumOfDirectionalLights; ++i)
        {
            directional[i].bind(index++);
        }
    }

    auto setPointMapping(ape::DepthMapping const & mapping)
        -> void
    {
        setTextures(mapping.getPointMapping(), point);
    }

    auto setSpotMapping(ape::DepthMapping const & mapping)
        -> void
    {
        setTextures(mapping.getSpotMapping(), spot);
    }

    auto setDirectionalMapping(ape::DepthMapping const & mapping)
        -> void
    {
        setTextures(mapping.getDirectionalMapping(), directional);
    }

    template<typename DepthMapType, typename TextureType>
    auto setTextures(
        std::vector<DepthMapType> const & source,
        UnsizedVectorUniform<Uniform<TextureType>> & target)
        -> void
    {
        for (auto i = 0; i < static_cast<int>(source.size()); ++i)
        {
            target[i] = source[i].getTexture();
        }
    }

private:

    int maxNumOfPointLights;

    int maxNumOfSpotLights;

    int maxNumOfDirectionalLights;

    int firstDepthMapUnit;

};

} // namespace glow
