#pragma once

#include "Lighting.hpp"
#include "ShaderProgram.hpp"
#include "UniformArray.hpp"

class AttenuationUniform
{

public:

    using ValueType = Attenuation;

public:

    AttenuationUniform(ShaderProgram const & program, std::string const & prefix)
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
        -> AttenuationUniform &
    {
        set(attenuation);

        return *this;
    }

public:

    Uniform<float> constant;

    Uniform<float> linear;

    Uniform<float> quadratic;

};

class LightColorUniform
{

public:

    using ValueType = Light::Color;

public:

    LightColorUniform(ShaderProgram const & program, std::string const & prefix)
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
        -> LightColorUniform &
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

    using ValueType = PointLight;

public:

    PointLightUniform(ShaderProgram const & program, std::string const & prefix)
        : position{program, prefix + ".position"}
        , color{program, prefix + ".color"}
        , attenuation{program, prefix + ".attenuation"}
        , isTurnedOn{program, prefix + ".isTurnedOn"}
    {
    }

    auto get() const
        -> ValueType
    {
        return {position.get(), attenuation.get(), color.get(), isTurnedOn.get()};
    }

    auto set(ValueType const & light)
        -> void
    {
        position = light.position;

        color = light.color;

        attenuation = light.attenuation;

        isTurnedOn = light.isTurnedOn;
    }

    auto operator = (ValueType const & light)
        -> PointLightUniform &
    {
        set(light);

        return *this;
    }

public:

    Uniform<glm::vec3> position;

    LightColorUniform color;

    AttenuationUniform attenuation;

    Uniform<bool> isTurnedOn;

};

class SpotLightUniform
{

public:

    using ValueType = SpotLight;

public:

    SpotLightUniform(ShaderProgram const & program, std::string const & prefix)
        : position{program, prefix + ".position"}
        , direction{program, prefix + ".direction"}
        , innerCutoffCosine{program, prefix + ".innerCutoffCosine"}
        , outerCutoffCosine{program, prefix + ".outerCutoffCosine"}
        , color{program, prefix + ".color"}
        , attenuation{program, prefix + ".attenuation"}
        , isTurnedOn{program, prefix + ".isTurnedOn"}
    {
    }

    auto get() const
        -> ValueType
    {
        return {
            position.get(),
            direction.get(),
            {glm::acos(innerCutoffCosine.get()), glm::acos(outerCutoffCosine.get())},
            attenuation.get(),
            color.get(),
            isTurnedOn.get()};
    }

    auto set(ValueType const & light)
        -> void
    {
        position = light.position;

        direction = light.direction;

        innerCutoffCosine = glm::cos(light.cutoff.inner);

        outerCutoffCosine = glm::cos(light.cutoff.outer);

        color = light.color;

        attenuation = light.attenuation;

        isTurnedOn = light.isTurnedOn;
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

    LightColorUniform color;

    AttenuationUniform attenuation;

    Uniform<bool> isTurnedOn;

};

class DirectionalLightUniform
{

public:

    using ValueType = DirectionalLight;

public:

    DirectionalLightUniform(ShaderProgram const & program, std::string const & prefix)
        : direction{program, prefix + ".direction"}
        , color{program, prefix + ".color"}
        , isTurnedOn{program, prefix + ".isTurnedOn"}
    {
    }

    auto get() const
        -> ValueType
    {
        return {direction.get(), color.get(), isTurnedOn.get()};
    }

    auto set(ValueType const & light)
        -> void
    {
        direction = light.direction;

        color = light.color;

        isTurnedOn = light.isTurnedOn;
    }

    auto operator = (ValueType const & light)
        -> DirectionalLightUniform &
    {
        set(light);

        return *this;
    }

public:

    Uniform<glm::vec3> direction;

    LightColorUniform color;

    Uniform<bool> isTurnedOn;

};

class LightingUniform
{

public:

    using ValueType = Lighting;

public:

    LightingUniform(ShaderProgram const & program, std::string prefix)
        : point{program, prefix + ".point"}
        , spot{program, prefix + ".spot"}
        , directional{program, prefix + ".directional"}
    {
    }

    auto get() const
        -> ValueType
    {
        return {point.get(), spot.get(), directional.get()};
    }

    auto set(ValueType const & lighting)
        -> void
    {
        point = lighting.point;

        spot = lighting.spot;

        directional = lighting.directional;
    }

    auto operator = (ValueType const & light)
        -> LightingUniform &
    {
        set(light);

        return *this;
    }

public:

    UniformArray<PointLightUniform> point;

    UniformArray<SpotLightUniform> spot;

    UniformArray<DirectionalLightUniform> directional;

};
