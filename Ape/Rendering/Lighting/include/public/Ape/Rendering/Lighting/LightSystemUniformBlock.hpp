#pragma once

#include <Ape/World/Scene/LightSystem.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/UniformBlock.hpp>
#include <Glow/Shader/UniformBlockMember.hpp>
#include <Glow/Shader/UniformArrayBlockMember.hpp>

namespace glow
{

template<>
class UniformBlockMember<ape::Attenuation>
{

public:

    using ValueType = ape::Attenuation;

public:

    UniformBlockMember(UniformBlock & block, std::string const name)
        : constant{block, std::string{name} + ".constant"}
        , quadratic{block, std::string{name} + ".quadratic"}
    {
    }

    auto set(ValueType const & attenuation, std::byte * const buffer)
        -> void
    {
        constant.set(attenuation.constant, buffer);

        quadratic.set(attenuation.quadratic, buffer);
    }

public:

    UniformBlockMember<float> constant;

    UniformBlockMember<float> quadratic;

};

template<>
class UniformBlockMember<ape::Light::Color>
{

public:

    using ValueType = ape::Light::Color;

public:

    UniformBlockMember(UniformBlock & block, std::string const name)
        : ambient{block, std::string{name} + ".ambient"}
        , diffuse{block, std::string{name} + ".diffuse"}
        , specular{block, std::string{name} + ".specular"}
    {
    }

    auto set(ValueType const & attenuation, std::byte * const buffer)
        -> void
    {
        ambient.set(attenuation.ambient, buffer);

        diffuse.set(attenuation.diffuse, buffer);

        specular.set(attenuation.specular, buffer);
    }

public:

    UniformBlockMember<glm::vec3> ambient;

    UniformBlockMember<glm::vec3> diffuse;

    UniformBlockMember<glm::vec3> specular;

};

template<>
class UniformBlockMember<ape::PointLight>
{

public:

    using ValueType = ape::PointLight;

public:

    UniformBlockMember(UniformBlock & block, std::string const name)
        : position{block, std::string{name} + ".position"}
        , color{block, std::string{name} + ".color"}
        , attenuation{block, std::string{name} + ".attenuation"}
        , isTurnedOn{block, std::string{name} + ".isTurnedOn"}
        , isCastingShadow{block, std::string{name} + ".isCastingShadow"}
    {
    }

    auto set(ValueType const & light, std::byte * const buffer)
        -> void
    {
        isTurnedOn.set(light.isTurnedOn(), buffer);

        if (not light.isTurnedOn())
        {
            // The shader will not need light data if it is turned off.
            return;
        }

        position.set(light.getPosition(), buffer);

        color.set(light.getColor(), buffer);

        attenuation.set(light.getAttenuation(), buffer);

        isCastingShadow.set(light.isCastingShadow(), buffer);
    }

public:

    UniformBlockMember<glm::vec3> position;

    UniformBlockMember<ape::Light::Color> color;

    UniformBlockMember<ape::Attenuation> attenuation;

    UniformBlockMember<bool> isTurnedOn;
    
    UniformBlockMember<bool> isCastingShadow;

};

template<>
class UniformBlockMember<ape::SpotLight>
{

public:

    using ValueType = ape::SpotLight;

public:

    UniformBlockMember(UniformBlock & block, std::string const name)
        : position{block, std::string{name} + ".position"}
        , direction{block, std::string{name} + ".direction"}
        , innerCutoffCosine{block, std::string{name} + ".innerCutoffCosine"}
        , outerCutoffCosine{block, std::string{name} + ".outerCutoffCosine"}
        , color{block, std::string{name} + ".color"}
        , attenuation{block, std::string{name} + ".attenuation"}
        , isTurnedOn{block, std::string{name} + ".isTurnedOn"}
        , isCastingShadow{block, std::string{name} + ".isCastingShadow"}
    {
    }

    auto set(ValueType const & light, std::byte * const buffer)
        -> void
    {
        isTurnedOn.set(light.isTurnedOn(), buffer);

        if (not light.isTurnedOn())
        {
            // The shader will not need light data if it is turned off.
            return;
        }

        position.set(light.getPosition(), buffer);

        // Normalization could be done in the vertex or fragment shader but there is no reason to
        // have that done millions of times for the same value, so hopefully doing this on the CPU
        // will save some performance.
        direction.set(glm::normalize(light.getDirection()), buffer);

        innerCutoffCosine.set(glm::cos(light.getCutoff().inner), buffer);

        outerCutoffCosine.set(glm::cos(light.getCutoff().outer), buffer);

        color.set(light.getColor(), buffer);

        attenuation.set(light.getAttenuation(), buffer);

        isCastingShadow.set(light.isCastingShadow(), buffer);
    }

public:

    UniformBlockMember<glm::vec3> position;

    UniformBlockMember<glm::vec3> direction;

    UniformBlockMember<float> innerCutoffCosine;

    UniformBlockMember<float> outerCutoffCosine;

    UniformBlockMember<ape::Light::Color> color;

    UniformBlockMember<ape::Attenuation> attenuation;

    UniformBlockMember<bool> isTurnedOn;
    
    UniformBlockMember<bool> isCastingShadow;

};

template<>
class UniformBlockMember<ape::DirectionalLight>
{

public:

    using ValueType = ape::DirectionalLight;

public:

    UniformBlockMember(UniformBlock & block, std::string const name)
        : direction{block, std::string{name} + ".direction"}
        , color{block, std::string{name} + ".color"}
        , isTurnedOn{block, std::string{name} + ".isTurnedOn"}
        , isCastingShadow{block, std::string{name} + ".isCastingShadow"}
    {
    }

    auto set(ValueType const & light, std::byte * const buffer)
        -> void
    {
        isTurnedOn.set(light.isTurnedOn(), buffer);

        if (not light.isTurnedOn())
        {
            // The shader will not need light data if it is turned off.
            return;
        }

        // Normalization could be done in the vertex or fragment shader but there is no reason to
        // have that done millions of times for the same value, so hopefully doing this on the CPU
        // will save some performance.
        direction.set(glm::normalize(light.getDirection()), buffer);

        color.set(light.getColor(), buffer);

        isCastingShadow.set(light.isCastingShadow(), buffer);
    }

public:

    UniformBlockMember<glm::vec3> direction;

    UniformBlockMember<ape::Light::Color> color;

    UniformBlockMember<bool> isTurnedOn;
    
    UniformBlockMember<bool> isCastingShadow;

};

template<>
class UniformBlockMember<ape::LightSystem>
{

public:

    using ValueType = ape::LightSystem;

public:

    UniformBlockMember(UniformBlock & block, std::string_view const name)
        : numOfPointLights{block, std::string{name} + ".numOfPointLights"}
        , point{block, std::string{name} + ".point", 15}
        , numOfSpotLights{block, std::string{name} + ".numOfSpotLights"}
        , spot{block, std::string{name} + ".spot", 8}
        , numOfDirectionalLights{block, std::string{name} + ".numOfDirectionalLights"}
        , directional{block, std::string{name} + ".directional", 4}
    {
    }

    auto set(ValueType const & lightSystem, std::byte * const buffer)
        -> void
    {
        numOfPointLights.set(static_cast<int>(lightSystem.point.size()), buffer);

        point.set(lightSystem.point, buffer);

        numOfSpotLights.set(static_cast<int>(lightSystem.spot.size()), buffer);

        spot.set(lightSystem.spot, buffer);

        numOfDirectionalLights.set(static_cast<int>(lightSystem.directional.size()), buffer);

        directional.set(lightSystem.directional, buffer);
    }

public:

    UniformBlockMember<int> numOfPointLights;

    UniformArrayBlockMember<ape::PointLight> point;

    UniformBlockMember<int> numOfSpotLights;

    UniformArrayBlockMember<ape::SpotLight> spot;

    UniformBlockMember<int> numOfDirectionalLights;

    UniformArrayBlockMember<ape::DirectionalLight> directional;

};

} // namespace glow

namespace ape
{

class LightSystemUniformBlock : public glow::UniformBlock
{

public:

    using ValueType = ape::DirectionalLight;

public:

    LightSystemUniformBlock(glow::ShaderProgram & shader, std::string_view const name)
        : UniformBlock{shader, name}
        , lightSystem{*this, "lightSystem"}
    {
    }

    auto set(LightSystem const & system, std::byte * const buffer)
    {
        lightSystem.set(system, buffer);
    }

public:

    glow::UniformBlockMember<ape::LightSystem> lightSystem;

};

} // namespace ape
