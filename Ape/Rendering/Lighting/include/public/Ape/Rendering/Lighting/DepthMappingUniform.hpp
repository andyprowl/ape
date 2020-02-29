#pragma once

#include <Ape/Rendering/Lighting/DepthMapping.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/Texture2dArrayUniform.hpp>
#include <Glow/Shader/TextureCubeUniform.hpp>
#include <Glow/Shader/UniformArray.hpp>

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

    UniformArray<Uniform<TextureCube>> point;

    Uniform<Texture2dArray> spot;

    Uniform<Texture2dArray> directional;

private:

    auto bindSamplers(ShaderProgram & program)
        -> void
    {
        auto const shaderBinder = bind(program);

        auto index = firstDepthMapUnit;

        for (auto i = 0; i < maxNumOfPointLights; ++i)
        {
            point[i].setTextureUnit(index++);
        }

        spot.setTextureUnit(index++);

        directional.setTextureUnit(index++);
    }

    auto setPointMapping(ape::DepthMapping const & mapping)
        -> void
    {
        setTextures(mapping.getPointMapping(), point);
    }

    auto setSpotMapping(ape::DepthMapping const & mapping)
        -> void
    {
        auto & spotMapping = mapping.getSpotMapping();

        auto & texture = spotMapping.getTexture();

        spot.set(texture);
    }

    auto setDirectionalMapping(ape::DepthMapping const & mapping)
        -> void
    {
        auto & directionalMapping = mapping.getDirectionalMapping();

        auto & texture = directionalMapping.getTexture();

        directional.set(texture);
    }

    template<typename DepthMapType, typename TextureType>
    auto setTextures(
        std::vector<DepthMapType> const & source,
        UniformArray<Uniform<TextureType>> & target)
        -> void
    {
        for (auto i = 0; i < static_cast<int>(source.size()); ++i)
        {
            auto const & texture = source[i].getTexture();

            target[i].set(texture);
        }
    }

private:

    int maxNumOfPointLights;

    int firstDepthMapUnit;

};

} // namespace glow
