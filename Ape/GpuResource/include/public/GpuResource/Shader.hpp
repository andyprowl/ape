#pragma once

#include <GpuResource/GpuResource.hpp>

#include <stdexcept>
#include <string>

namespace ape
{

class CouldNotCompileShader : public std::logic_error
{

public:

    explicit CouldNotCompileShader(std::string message)
        : logic_error{"Failed to compile vertex shader: " + std::move(message)}
    {
    }

};

class Shader
{

public:

    enum class Type
    {
        unknown,
        vertex,
        geometry,
        fragment
    };

public:

    auto getId() const
        -> unsigned int;

    auto compile(std::string const & newSourceCode)
        -> void;

    auto getSourceCode() const
        -> std::string;

    auto getType() const
        -> Type;

    auto release()
        -> unsigned int;

protected:

    explicit Shader(Type type, std::string const & sourceCode);

private:

    auto createShaderResource(Type type) const
        -> GpuResource;

private:

    GpuResource resource;

};

} // namespace ape
