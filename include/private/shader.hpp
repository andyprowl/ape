#pragma once

#include <glm/mat4x4.hpp>

#include <stdexcept>
#include <string>
#include <vector>

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

class ShaderProgram
{

public:

    ShaderProgram(
        std::string const & vertexShaderFilename,
        std::string const & fragmentShaderFilename);

    auto use() const
        -> void;

    auto getBool(std::string const & name) const
        -> bool;

    auto getFloat(std::string const & name) const
        -> float;

    auto getMat4(std::string const & name) const
        -> glm::mat4;

    auto set(std::string const & name, bool value) const
        -> void;

    auto set(std::string const & name, int value) const
        -> void;

    auto set(std::string const & name, float value) const
        -> void;

    auto set(std::string const & name, std::array<float, 2u> const & value) const
        -> void;

    auto set(std::string const & name, std::array<float, 3u> const & value) const
        -> void;

    auto set(std::string const & name, std::array<float, 4u> const & value) const
        -> void;

    auto set(std::string const & name, glm::mat4 const & value) const
        -> void;

    auto bindFragmentSamplers(std::vector<std::string> const & samplerIds) const
        -> void;

private:

    int id;

};

auto readShader(std::string const & filename)
    -> std::string;

auto compileShader(std::string const & filename, int const shaderType)
    -> int;

auto compileVertexShader(std::string const & filename)
    -> int;

auto compileFragmentShader(std::string const & filename)
    -> int;

auto linkShaderProgram(int const vertexShaderId, int const fragmentShaderId)
    -> int;

auto makeShaderProgram(
    std::string const & vertexShaderFilename,
    std::string const & fragmentShaderFilename)
    -> int;
