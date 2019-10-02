#pragma once

#include <Core/Uniform.hpp>

#include <glm/mat4x4.hpp>

#include <stdexcept>
#include <string>
#include <vector>

namespace ape
{

class CouldNotOpenShaderFile : public std::logic_error
{

public:

    CouldNotOpenShaderFile(std::string filename)
        : logic_error{"Failed to open shader file '" + std::move(filename) + "': "}
    {
    }

};

class CouldNotCompileShader : public std::logic_error
{

public:

    CouldNotCompileShader(std::string filename, std::string message)
        : logic_error{
            "Failed to compile shader '" +
            std::move(filename) +
            "': " +
            std::move(message)}
    {
    }

};

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

    ShaderProgram(
        std::string const & vertexShaderFilename,
        std::string const & geometryShaderFilename,
        std::string const & fragmentShaderFilename);

    auto use() const
        -> void;

    template<typename T>
    auto getUniform(std::string const & name) const
        -> Uniform<T>
    {
        auto const location = getUniformLocation(name);

        return Uniform<T>{id, location};
    }

private:

    auto getUniformLocation(std::string const & name) const
        -> int;

private:

    int id;

};

auto readShader(std::string const & filename)
    -> std::string;

auto compileShader(std::string const & filename, int const shaderType)
    -> int;

auto compileVertexShader(std::string const & filename)
    -> int;

auto compileGeometryShader(std::string const & filename)
    -> int;

auto compileFragmentShader(std::string const & filename)
    -> int;

auto linkShaderProgram(int const vertexShaderId, int const fragmentShaderId)
    -> int;

auto makeShaderProgram(
    std::string const & vertexShaderFilename,
    std::string const & geometryShaderFilename,
    std::string const & fragmentShaderFilename)
    -> int;

} // namespace ape
