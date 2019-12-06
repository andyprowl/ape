#pragma once

#include <Glow/GpuResource/GpuResource.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace glow
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

    auto compile(std::string_view newSourceCode)
        -> void;

    auto getSourceCode() const
        -> std::string;

    auto getType() const
        -> Type;

    auto release()
        -> GpuResource::Id;

    auto setLabel(std::string_view label)
        -> void;

protected:

    Shader(Type type, std::string_view sourceCode, std::string_view label = "");

private:

    auto createShaderResource(Type type) const
        -> GpuResource;

private:

    GpuResource resource;

};

} // namespace glow
