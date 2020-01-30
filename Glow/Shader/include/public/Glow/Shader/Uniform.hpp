#pragma once

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <string_view>
#include <vector>

namespace glow
{

class ShaderProgram;

} // namespace glow

namespace glow::detail
{

class BasicUniform
{

public:

    BasicUniform(ShaderProgram & shader, std::string_view name);
    
    auto getShader() const
        -> ShaderProgram &;

    auto getLocation() const
        -> int;

private:

    auto getUniformLocation(std::string_view name) const
        -> int;

private:

    ShaderProgram * shader;
    
    int location;

};

} // namespace glow::detail

namespace glow
{

template<typename T>
class Uniform : public detail::BasicUniform
{

public:

    using ValueType = T;

public:

    using BasicUniform::BasicUniform;

    Uniform(ShaderProgram & program, std::string_view name, ValueType const & value)
        : BasicUniform{program, name}
    {
        auto const shaderBinder = glow::bind(program);

        set(value);
    }

    auto get() const
        -> ValueType;

    auto set(ValueType const & value)
        -> void;

};

} // namespace glow
