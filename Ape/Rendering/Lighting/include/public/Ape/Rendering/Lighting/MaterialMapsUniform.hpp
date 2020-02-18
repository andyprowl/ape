#pragma once

#include <Ape/World/Model/Material.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/TextureUniform.hpp>

namespace ape
{

class MaterialMapsUniform
{

public:

    using ValueType = ape::Material;

public:

    MaterialMapsUniform(
        glow::ShaderProgram & program,
        std::string const & prefix,
        int const diffuseMapUnit,
        int const specularMapUnit,
        int const normalMapUnit)
        : diffuseMap{program, prefix + ".diffuseMap", diffuseMapUnit}
        , specularMap{program, prefix + ".specularMap", specularMapUnit}
        , normalMap{program, prefix + ".normalMap", normalMapUnit}
    {
    }

    auto set(ape::Material const & material)
        -> void
    {
        setDiffuseMap(material);
    
        setSpecularMap(material);

        setNormalMap(material);
    }

private:

    auto setDiffuseMap(ape::Material const & material)
        -> void
    {
        if (material.diffuseMap != nullptr)
        {
            diffuseMap.set(*material.diffuseMap);
        }
    }

    auto setSpecularMap(ape::Material const & material)
        -> void
    {
        if (material.specularMap != nullptr)
        {
            specularMap.set(*material.specularMap);
        }
    }

    auto setNormalMap(ape::Material const & material)
        -> void
    {
        if (material.normalMap != nullptr)
        {
            normalMap.set(*material.normalMap);
        }
    }

public:

    glow::Uniform<glow::Texture> diffuseMap;

    glow::Uniform<glow::Texture> specularMap;

    glow::Uniform<glow::Texture> normalMap;

};

} // namespace ape
