#pragma once

#include <Glow/Shader/FragmentShader.hpp>
#include <Glow/Shader/GeometryShader.hpp>
#include <Glow/Shader/Uniform.hpp>
#include <Glow/Shader/VertexShader.hpp>

#include <Glow/GpuResource/GpuResource.hpp>

#include <optional>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace glow
{

class CouldNotLinkShaderProgram : public std::logic_error
{

public:

    explicit CouldNotLinkShaderProgram(std::string message)
        : logic_error{"Failed to link shader program: " + std::move(message)}
    {
    }

};

class UniformNotFound : public std::logic_error
{

public:

    explicit UniformNotFound(std::string name)
        : logic_error{"The uniform '" + std::move(name) + "' was not found"}
    {
    }

};

class ShaderProgram
{

public:

    ShaderProgram(VertexShader vertexShader, FragmentShader fragmentShader);

    ShaderProgram(
        VertexShader vertexShader,
        std::optional<GeometryShader> geometryShader,
        FragmentShader fragmentShader);

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    template<typename T>
    auto getUniform(std::string_view name) const
        -> Uniform<T>
    {
        auto const id = resource.get();

        auto const location = getUniformLocation(name);

        return Uniform<T>{id, location};
    }

    auto getVertexShader() const
        -> VertexShader const &;

    auto getGeometryShader() const
        -> std::optional<GeometryShader> const &;

    auto getFragmentShader() const
        -> FragmentShader const &;

private:

    auto link()
        -> void;

    auto getUniformLocation(std::string_view name) const
        -> int;

private:

    GpuResource resource;

    VertexShader vertexShader;
    
    std::optional<GeometryShader> geometryShader;
    
    FragmentShader fragmentShader;

};

} // namespace glow
