#pragma once

#include <GpuResource/GpuResource.hpp>

#include <stdexcept>
#include <string>

namespace ape
{

class CouldNotCompileShader : public std::logic_error
{

public:

    CouldNotCompileShader(std::string compilerMessage, std::string type)
        : logic_error{
            "Failed to compile " +
            std::move(type) +
            " shader: " +
            std::move(compilerMessage)}
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
        -> GpuResource::Id;

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
