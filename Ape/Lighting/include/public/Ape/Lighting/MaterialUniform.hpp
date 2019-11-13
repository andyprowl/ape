#pragma once

#include <Ape/Shader/ShaderProgram.hpp>
#include <Ape/Shader/TextureUniform.hpp>

#include <Ape/Model/Material.hpp>

namespace ape
{

template<>
class Uniform<Material>
{

public:

    using ValueType = Material;

public:

    Uniform(
        ShaderProgram & program,
        std::string const & prefix,
        int const diffuseMapUnit,
        int const specularMapUnit)
        : ambient{program, prefix + ".ambient"}
        , shininess{program, prefix + ".shininess"}
        , diffuseMap{program, prefix + ".diffuse", diffuseMapUnit}
        , specularMap{program, prefix + ".specular", specularMapUnit}
    {
    }

    auto set(Material const & material)
        -> void
    {
        ambient = material.ambient;

        shininess = material.shininess;

        if (material.diffuseMap != nullptr)
        {
            diffuseMap = *material.diffuseMap;
        }
    
        if (material.specularMap != nullptr)
        {
            specularMap = *material.specularMap;
        }
    }

    auto operator = (ValueType const & material)
        -> Uniform &
    {
        set(material);

        return *this;
    }

public:

    Uniform<glm::vec3> ambient;

    Uniform<float> shininess;

    Uniform<Texture> diffuseMap;

    Uniform<Texture> specularMap;

};

} // namespace ape
