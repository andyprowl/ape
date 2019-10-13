#pragma once

#include <Scene/Lighting.hpp>

#include <GpuResource/ShaderProgram.hpp>
#include <GpuResource/VectorUniform.hpp>

namespace ape
{

template<>
class Uniform<Attenuation>
{

public:

    using ValueType = Attenuation;

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
class Uniform<Light::Color>
{

public:

    using ValueType = Light::Color;

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

    using ValueType = PointLight;

public:

    PointLightUniform(ShaderProgram & program, std::string const & prefix)
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

    Uniform<Light::Color> color;

    Uniform<Attenuation> attenuation;

    Uniform<bool> isTurnedOn;

};

class SpotLightUniform
{

public:

    using ValueType = SpotLight;

public:

    SpotLightUniform(ShaderProgram & program, std::string const & prefix)
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
        position = light.getPosition();

        direction = light.getDirection();

        innerCutoffCosine = glm::cos(light.getCutoff().inner);

        outerCutoffCosine = glm::cos(light.getCutoff().outer);

        color = light.color;

        attenuation = light.getAttenuation();

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

    Uniform<Light::Color> color;

    Uniform<Attenuation> attenuation;

    Uniform<bool> isTurnedOn;

};

class DirectionalLightUniform
{

public:

    using ValueType = DirectionalLight;

public:

    DirectionalLightUniform(ShaderProgram & program, std::string const & prefix)
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

    Uniform<Light::Color> color;

    Uniform<bool> isTurnedOn;

};

template<>
class Uniform<Lighting>
{

public:

    using ValueType = Lighting;

public:

    Uniform(ShaderProgram & program, std::string const & prefix)
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

} // namespace ape
