#pragma once

#include <Ape/World/Scene/Fog.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/Uniform.hpp>

namespace glow
{

template<>
class Uniform<ape::Fog>
{

public:

    using ValueType = ape::Fog;

public:

    Uniform(ShaderProgram & program, std::string const & prefix)
        : density{program, prefix + ".density"}
        , color{program, prefix + ".color"}
    {
    }

    auto get() const
        -> ValueType
    {
        return {density.get(), color.get()};
    }

    auto set(ValueType const & fog)
        -> void
    {
        density = fog.density;

        color = fog.color;
    }

    auto operator = (ValueType const & attenuation)
        -> Uniform &
    {
        set(attenuation);

        return *this;
    }

public:

    Uniform<float> density;

    Uniform<glm::vec3> color;

};

} // namespace glow
