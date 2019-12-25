#pragma once

#include <Ape/World/Model/Material.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/TextureUniform.hpp>

namespace glow
{

template<>
class Uniform<ape::Material>
{

public:

    using ValueType = ape::Material;

public:

    Uniform(
        ShaderProgram & program,
        std::string const & prefix,
        int const diffuseMapUnit,
        int const specularMapUnit,
        int const normalMapUnit)
        : ambient{program, prefix + ".ambient"}
        , shininess{program, prefix + ".shininess"}
        , hasDiffuseMap{program, prefix + ".hasDiffuseMap"}
        , diffuseMap{program, prefix + ".diffuseMap", diffuseMapUnit}
        , hasSpecularMap{program, prefix + ".hasSpecularMap"}
        , specularMap{program, prefix + ".specularMap", specularMapUnit}
        , hasNormalMap{program, prefix + ".hasNormalMap"}
        , normalMap{program, prefix + ".normalMap", normalMapUnit}
    {
    }

    auto set(ape::Material const & material)
        -> void
    {
        ambient = material.ambient;

        shininess = material.shininess;

        setDiffuseMap(material);
    
        setSpecularMap(material);

        setNormalMap(material);
    }

    auto operator = (ValueType const & material)
        -> Uniform &
    {
        set(material);

        return *this;
    }

private:

    auto setDiffuseMap(ape::Material const & material)
        -> void
    {
        hasDiffuseMap = (material.diffuseMap != nullptr);

        if (material.diffuseMap != nullptr)
        {
            diffuseMap = *material.diffuseMap;
        }
    }

    auto setSpecularMap(ape::Material const & material)
        -> void
    {
        hasSpecularMap = (material.specularMap != nullptr);

        if (material.specularMap != nullptr)
        {
            specularMap = *material.specularMap;
        }
    }

    auto setNormalMap(ape::Material const & material)
        -> void
    {
        hasNormalMap = (material.normalMap != nullptr);

        if (material.normalMap != nullptr)
        {
            normalMap = *material.normalMap;
        }
    }

public:

    Uniform<glm::vec3> ambient;

    Uniform<float> shininess;

    Uniform<bool> hasDiffuseMap;

    Uniform<Texture> diffuseMap;

    Uniform<bool> hasSpecularMap;

    Uniform<Texture> specularMap;

    Uniform<bool> hasNormalMap;

    Uniform<Texture> normalMap;

};

} // namespace glow
