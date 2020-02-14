#pragma once

#include <Ape/World/Scene/LightSystem.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/UniformArray.hpp>

namespace glow
{

template<>
class Uniform<ape::Attenuation>
{

public:

    using ValueType = ape::Attenuation;

public:

    Uniform(ShaderProgram & program, std::string const & prefix)
        : constant{program, prefix + ".constant"}
        , linear{program, prefix + ".linear"}
        , quadratic{program, prefix + ".quadratic"}
    {
    }

    auto get() const
        -> ValueType
    {
        return {constant.get(), linear.get(), quadratic.get()};
    }

    auto set(ValueType const & attenuation)
        -> void
    {
        constant.set(attenuation.constant);

        linear.set(attenuation.linear);

        quadratic.set(attenuation.quadratic);
    }

public:

    Uniform<float> constant;

    Uniform<float> linear;

    Uniform<float> quadratic;

};

template<>
class Uniform<ape::Light::Color>
{

public:

    using ValueType = ape::Light::Color;

public:

    Uniform(ShaderProgram & program, std::string const & prefix)
        : ambient{program, prefix + ".ambient"}
        , diffuse{program, prefix + ".diffuse"}
        , specular{program, prefix + ".specular"}
    {
    }

    auto get() const
        -> ValueType
    {
        return {ambient.get(), diffuse.get(), specular.get()};
    }

    auto set(ValueType const & color)
        -> void
    {
        ambient.set(color.ambient);

        diffuse.set(color.diffuse);

        specular.set(color.specular);
    }

public:

    Uniform<glm::vec3> ambient;

    Uniform<glm::vec3> diffuse;

    Uniform<glm::vec3> specular;

};

class PointLightUniform
{

public:

    using ValueType = ape::PointLight;

public:

    PointLightUniform(ShaderProgram & program, std::string const & prefix)
        : position{program, prefix + ".position"}
        , color{program, prefix + ".color"}
        , attenuation{program, prefix + ".attenuation"}
        , isTurnedOn{program, prefix + ".isTurnedOn"}
        , isCastingShadow{program, prefix + ".isCastingShadow"}
    {
    }

    auto set(ValueType const & light)
        -> void
    {
        isTurnedOn.set(light.isTurnedOn());

        if (not isTurnedOn.get())
        {
            // The shader will not need light data if it is turned off.
            return;
        }

        position.set(light.getPosition());

        color.set(light.getColor());

        attenuation.set(light.getAttenuation());

        isCastingShadow.set(light.isCastingShadow());
    }

public:

    Uniform<glm::vec3> position;

    Uniform<ape::Light::Color> color;

    Uniform<ape::Attenuation> attenuation;

    Uniform<bool> isTurnedOn;

    Uniform<bool> isCastingShadow;

};

class SpotLightUniform
{

public:

    using ValueType = ape::SpotLight;

public:

    SpotLightUniform(ShaderProgram & program, std::string const & prefix)
        : position{program, prefix + ".position"}
        , direction{program, prefix + ".direction"}
        , innerCutoffCosine{program, prefix + ".innerCutoffCosine"}
        , outerCutoffCosine{program, prefix + ".outerCutoffCosine"}
        , color{program, prefix + ".color"}
        , attenuation{program, prefix + ".attenuation"}
        , isTurnedOn{program, prefix + ".isTurnedOn"}
        , isCastingShadow{program, prefix + ".isCastingShadow"}
    {
    }

    auto set(ValueType const & light)
        -> void
    {
        isTurnedOn.set(light.isTurnedOn());

        if (not isTurnedOn.get())
        {
            // The shader will not need light data if it is turned off.
            return;
        }

        position.set(light.getPosition());

        // Normalization could be done in the vertex or fragment shader but there is no reason to
        // have that done millions of times for the same value, so hopefully doing this on the CPU
        // will save some performance.
        direction.set(glm::normalize(light.getDirection()));

        innerCutoffCosine.set(glm::cos(light.getCutoff().inner));

        outerCutoffCosine.set(glm::cos(light.getCutoff().outer));

        color.set(light.getColor());

        attenuation.set(light.getAttenuation());

        isCastingShadow.set(light.isCastingShadow());
    }

public:

    Uniform<glm::vec3> position;

    Uniform<glm::vec3> direction;

    Uniform<float> innerCutoffCosine;

    Uniform<float> outerCutoffCosine;

    Uniform<ape::Light::Color> color;

    Uniform<ape::Attenuation> attenuation;

    Uniform<bool> isTurnedOn;

    Uniform<bool> isCastingShadow;

};

class DirectionalLightUniform
{

public:

    using ValueType = ape::DirectionalLight;

public:

    DirectionalLightUniform(ShaderProgram & program, std::string const & prefix)
        : direction{program, prefix + ".direction"}
        , color{program, prefix + ".color"}
        , isTurnedOn{program, prefix + ".isTurnedOn"}
        , isCastingShadow{program, prefix + ".isCastingShadow"}
    {
    }

    auto set(ValueType const & light)
        -> void
    {
        isTurnedOn.set(light.isTurnedOn());

        if (not isTurnedOn.get())
        {
            // The shader will not need light data if it is turned off.
            return;
        }

        // Normalization could be done in the vertex or fragment shader but there is no reason to
        // have that done millions of times for the same value, so hopefully doing this on the CPU
        // will save some performance.
        direction.set(glm::normalize(light.getDirection()));

        color.set(light.getColor());

        isCastingShadow.set(light.isCastingShadow());
    }

public:

    Uniform<glm::vec3> direction;

    Uniform<ape::Light::Color> color;

    Uniform<bool> isTurnedOn;

    Uniform<bool> isCastingShadow;

};

template<>
class Uniform<ape::LightSystem>
{

public:

    using ValueType = ape::LightSystem;

public:

    Uniform(ShaderProgram & program, std::string const & prefix)
        : point{program, prefix + ".point"}
        , numOfPointLights{program, prefix + ".numOfPointLights"}
        , spot{program, prefix + ".spot"}
        , numOfSpotLights{program, prefix + ".numOfSpotLights"}
        , directional{program, prefix + ".directional"}
        , numOfDirectionalLights{program, prefix + ".numOfDirectionalLights"}
    {
    }

    auto set(ValueType const & lightSystem)
        -> void
    {
        point.set(lightSystem.point);

        numOfPointLights.set(static_cast<int>(lightSystem.point.size()));

        spot.set(lightSystem.spot);

        numOfSpotLights.set(static_cast<int>(lightSystem.spot.size()));

        directional.set(lightSystem.directional);

        numOfDirectionalLights.set(static_cast<int>(lightSystem.directional.size()));
    }

public:

    UniformArray<PointLightUniform> point;

    Uniform<int> numOfPointLights;

    UniformArray<SpotLightUniform> spot;

    Uniform<int> numOfSpotLights;

    UniformArray<DirectionalLightUniform> directional;

    Uniform<int> numOfDirectionalLights;

};

} // namespace glow
