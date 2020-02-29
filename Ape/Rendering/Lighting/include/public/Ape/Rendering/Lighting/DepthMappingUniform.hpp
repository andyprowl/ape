#pragma once

#include <Ape/Rendering/Lighting/DepthMapping.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/Texture2dArrayUniform.hpp>
#include <Glow/Shader/TextureCubeArrayUniform.hpp>
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

    Uniform<TextureCubeArray> point;

    Uniform<Texture2dArray> spot;

    Uniform<Texture2dArray> directional;

private:

    auto bindSamplers(ShaderProgram & program)
        -> void
    {
        auto const shaderBinder = bind(program);

        auto index = firstDepthMapUnit;

        point.setTextureUnit(index++);

        spot.setTextureUnit(index++);

        directional.setTextureUnit(index++);
    }

    auto setPointMapping(ape::DepthMapping const & mapping)
        -> void
    {
        auto & pointMapping = mapping.getPointMapping();

        auto & texture = pointMapping.getTexture();

        point.set(texture);
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

private:

    int firstDepthMapUnit;

};

} // namespace glow
