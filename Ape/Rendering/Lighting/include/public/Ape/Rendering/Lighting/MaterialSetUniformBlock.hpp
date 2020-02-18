#pragma once

#include <Ape/World/Model/Material.hpp>

#include <Glow/Shader/UniformBlock.hpp>
#include <Glow/Shader/UniformArrayBlockMember.hpp>

namespace glow
{

template<>
class UniformBlockMember<ape::Material *>
{

public:

    using ValueType = ape::Material *;

public:

    UniformBlockMember(UniformBlock & block, std::string const & prefix)
        : ambient{block, prefix + ".ambient"}
        , shininess{block, prefix + ".shininess"}
        , hasDiffuseMap{block, prefix + ".hasDiffuseMap"}
        , hasSpecularMap{block, prefix + ".hasSpecularMap"}
        , hasNormalMap{block, prefix + ".hasNormalMap"}
    {
    }

    auto set(ape::Material const * material, std::byte * const buffer)
        -> void
    {
        ambient.set(material->ambient, buffer);

        shininess.set(material->shininess, buffer);

        hasDiffuseMap.set((material->diffuseMap != nullptr), buffer);

        hasSpecularMap.set((material->specularMap != nullptr), buffer);

        hasNormalMap.set((material->normalMap != nullptr), buffer);
    }

public:

    UniformBlockMember<glm::vec3> ambient;

    UniformBlockMember<float> shininess;

    UniformBlockMember<bool> hasDiffuseMap;

    UniformBlockMember<bool> hasSpecularMap;

    UniformBlockMember<bool> hasNormalMap;

};

} // namespace glow

namespace ape
{

class MaterialSetUniformBlock : public glow::UniformBlock
{

public:

    using ValueType = std::vector<Material *>;

public:

    MaterialSetUniformBlock(glow::ShaderProgram & shader, std::string_view const name)
        : UniformBlock{shader, name}
        , numOfMaterials{*this, "numOfMaterials"}
        , materialSet{*this, "materials", 1024}
    {
    }

    auto set(ValueType const & materials, std::byte * const buffer)
    {
        numOfMaterials.set(static_cast<int>(materials.size()), buffer);

        materialSet.set(materials, buffer);
    }

public:

    glow::UniformBlockMember<int> numOfMaterials;

    glow::UniformArrayBlockMember<ape::Material *> materialSet;

};

} // namespace ape
