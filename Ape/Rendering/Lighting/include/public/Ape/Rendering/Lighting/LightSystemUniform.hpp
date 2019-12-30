#pragma once

#include <Ape/World/Scene/LightSystem.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/VectorUniform.hpp>

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
        constant = attenuation.constant;

        linear = attenuation.linear;

        quadratic = attenuation.quadratic;
    }

    auto operator = (ValueType const & attenuation)
        -> Uniform &
    {
        set(attenuation);

        return *this;
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
        ambient = color.ambient;

        diffuse = color.diffuse;

        specular = color.specular;
    }

    auto operator = (ValueType const & color)
        -> Uniform &
    {
        set(color);

        return *this;
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
        isTurnedOn = light.isTurnedOn();

        if (!isTurnedOn)
        {
            // The shader will not need light data if it is turned off.
            return;
        }

        position = light.getPosition();

        color = light.getColor();

        attenuation = light.getAttenuation();

        isCastingShadow = light.isCastingShadow();
    }

    auto operator = (ValueType const & light)
        -> PointLightUniform &
    {
        set(light);

        return *this;
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
        isTurnedOn = light.isTurnedOn();

        if (!isTurnedOn)
        {
            // The shader will not need light data if it is turned off.
            return;
        }

        position = light.getPosition();

        // Normalization could be done in the vertex or fragment shader but there is no reason to
        // have that done millions of times for the same value, so hopefully doing this on the CPU
        // will save some performance.
        direction = glm::normalize(light.getDirection());

        innerCutoffCosine = glm::cos(light.getCutoff().inner);

        outerCutoffCosine = glm::cos(light.getCutoff().outer);

        color = light.getColor();

        attenuation = light.getAttenuation();

        isCastingShadow = light.isCastingShadow();
    }

    auto operator = (ValueType const & light)
        -> SpotLightUniform &
    {
        set(light);

        return *this;
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
        isTurnedOn = light.isTurnedOn();

        if (!isTurnedOn)
        {
            // The shader will not need light data if it is turned off.
            return;
        }

        // Normalization could be done in the vertex or fragment shader but there is no reason to
        // have that done millions of times for the same value, so hopefully doing this on the CPU
        // will save some performance.
        direction = glm::normalize(light.getDirection());

        color = light.getColor();

        isCastingShadow = light.isCastingShadow();
    }

    auto operator = (ValueType const & light)
        -> DirectionalLightUniform &
    {
        set(light);

        return *this;
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
        , spot{program, prefix + ".spot"}
        , directional{program, prefix + ".directional"}
    {
    }

    auto set(ValueType const & lightSystem)
        -> void
    {
        point = lightSystem.point;

        spot = lightSystem.spot;

        directional = lightSystem.directional;
    }

    auto operator = (ValueType const & light)
        -> Uniform &
    {
        set(light);

        return *this;
    }

public:

    SizedVectorUniform<PointLightUniform> point;

    SizedVectorUniform<SpotLightUniform> spot;

    SizedVectorUniform<DirectionalLightUniform> directional;

};

} // namespace glow
