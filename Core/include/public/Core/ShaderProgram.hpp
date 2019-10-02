#pragma once

#include <Core/FragmentShader.hpp>
#include <Core/GeometryShader.hpp>
#include <Core/VertexShader.hpp>

#include <Core/Uniform.hpp>

#include <glm/mat4x4.hpp>

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace ape
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

    ShaderProgram(ShaderProgram const & rhs) = delete;

    ShaderProgram(ShaderProgram && rhs) noexcept;

    auto operator = (ShaderProgram const & rhs)
        -> ShaderProgram & = delete;

    auto operator = (ShaderProgram && rhs) noexcept
        -> ShaderProgram &;

    ~ShaderProgram();

    auto use() const
        -> void;

    template<typename T>
    auto getUniform(std::string const & name) const
        -> Uniform<T>
    {
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

    auto destroy()
        -> void;

    auto getUniformLocation(std::string const & name) const
        -> int;

private:

    unsigned int id;

    VertexShader vertexShader;
    
    std::optional<GeometryShader> geometryShader;
    
    FragmentShader fragmentShader;

};

auto makeShaderProgram(
    std::string const & vertexShaderFilename,
    std::string const & geometryShaderFilename,
    std::string const & fragmentShaderFilename)
    -> int;

} // namespace ape
