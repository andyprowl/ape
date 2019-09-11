#include "shader.hpp"

#include "glfw.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <array>

namespace
{

auto checkShaderCompilationOutcome(int const shaderId, std::string const & filename)
    -> void
{
    auto success = int{};

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if (success)
    {
        std::cout << "Shader << '" << filename << "' successfully compiled\n";

        return;
    }
    
    auto infoLog = std::array<char, 512>{'\0'};

    glGetShaderInfoLog(shaderId, sizeof(infoLog), nullptr, infoLog.data());

    throw CouldNotCompileShader{filename, infoLog.data()};
}

auto checkShaderLinkingOutcome(int const shaderId)
    -> void
{
    auto success = int{};

    glGetProgramiv(shaderId, GL_LINK_STATUS, &success);

    if (success)
    {
        std::cout << "Shader program successfully linked\n";

        return;
    }

    auto infoLog = std::array<char, 512>{'\0'};

    glGetProgramInfoLog(shaderId, sizeof(infoLog), nullptr, infoLog.data());

    throw CouldNotLinkShaderProgram{infoLog.data()};
}

} // unnamed namespace

auto readShader(std::string const & filename)
    -> std::string
{
    auto shaderFile = std::ifstream{
        std::string{shaderFolder} + "\\" + filename,
        std::ifstream::ate | std::ifstream::binary};

    const auto fileSize = static_cast<std::size_t>(shaderFile.tellg());

    shaderFile.seekg(0, std::ifstream::beg);

    auto data = std::string(fileSize + 1, '\0');

    shaderFile.read(&data[0], fileSize);

    return data;
}

auto compileShader(std::string const & filename, int const shaderType)
    -> int
{
    auto const shader = readShader(filename);

    auto const shaderId = glCreateShader(shaderType);
    
    auto const shaderCode = shader.data();

    glShaderSource(shaderId, 1, &shaderCode, nullptr);

    glCompileShader(shaderId);

    checkShaderCompilationOutcome(shaderId, filename);

    return shaderId;
}

auto compileVertexShader(std::string const & filename)
    -> int
{
    return compileShader(filename, GL_VERTEX_SHADER);
}

auto compileFragmentShader(std::string const & filename)
    -> int
{
    return compileShader(filename, GL_FRAGMENT_SHADER);
}

auto linkShaderProgram(int const vertexShaderId, int const fragmentShaderId)
    -> int
{
    const auto shaderProgramId = glCreateProgram();

    glAttachShader(shaderProgramId, vertexShaderId);

    glAttachShader(shaderProgramId, fragmentShaderId);

    glLinkProgram(shaderProgramId);

    checkShaderLinkingOutcome(shaderProgramId);

    glDeleteShader(vertexShaderId);

    glDeleteShader(fragmentShaderId);

    return shaderProgramId;
}

auto makeShaderProgram(
    std::string const & vertexShaderFilename,
    std::string const & fragmentShaderFilename)
    -> int
{
    const auto vertexShaderId = compileVertexShader(vertexShaderFilename);

    const auto fragmentShaderId = compileFragmentShader(fragmentShaderFilename);

    return linkShaderProgram(vertexShaderId, fragmentShaderId);
}

ShaderProgram::ShaderProgram(
    std::string const & vertexShaderFilename,
    std::string const & fragmentShaderFilename)
    : id{makeShaderProgram(vertexShaderFilename, fragmentShaderFilename)}
{
}

auto ShaderProgram::use() const
    -> void
{
    glUseProgram(id);
}

auto ShaderProgram::getBool(std::string const & name) const
    -> bool
{
    const auto location = glGetUniformLocation(id, name.c_str());

    auto value = int{};

    glGetUniformiv(id, location, &value);

    return (value != 0);
}

auto ShaderProgram::getFloat(std::string const & name) const
    -> float
{
    const auto location = glGetUniformLocation(id, name.c_str());

    auto value = float{};

    glGetUniformfv(id, location, &value);

    return value;
}

auto ShaderProgram::getMat4(std::string const & name) const
    -> glm::mat4
{
    const auto location = glGetUniformLocation(id, name.c_str());

    auto value = std::array<float, 16>{0};

    glGetUniformfv(id, location, value.data());

    return glm::mat4{
        value[0], value[1], value[2], value[3],
        value[4], value[5], value[6], value[7],
        value[8], value[9], value[10], value[11],
        value[12], value[13], value[14], value[15]};
}

auto ShaderProgram::set(std::string const & name, bool const value) const
    -> void
{
    const auto location = glGetUniformLocation(id, name.c_str());

    glUniform1i(location, value);
}

auto ShaderProgram::set(std::string const & name, int const value) const
    -> void
{
    const auto location = glGetUniformLocation(id, name.c_str());

    glUniform1i(location, value);
}

auto ShaderProgram::set(std::string const & name, float const value) const
    -> void
{
    const auto location = glGetUniformLocation(id, name.c_str());

    glUniform1f(location, value);
}

auto ShaderProgram::set(std::string const & name, std::array<float, 2u> const & value) const
    -> void
{
    const auto location = glGetUniformLocation(id, name.c_str());

    glUniform2f(location, value[0], value[1]);
}

auto ShaderProgram::set(std::string const & name, std::array<float, 3u> const & value) const
    -> void
{
    const auto location = glGetUniformLocation(id, name.c_str());

    glUniform3f(location, value[0], value[1], value[2]);
}

auto ShaderProgram::set(std::string const & name, std::array<float, 4u> const & value) const
    -> void
{
    const auto location = glGetUniformLocation(id, name.c_str());

    glUniform4f(location, value[0], value[1], value[2], value[3]);
}

auto ShaderProgram::set(std::string const & name, glm::mat4 const & value) const
    -> void
{
    const auto location = glGetUniformLocation(id, name.c_str());

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

auto ShaderProgram::bindFragmentSamplers(std::vector<std::string> const & samplerIds) const
    -> void
{
    use();

    for (auto i = 0; i < static_cast<int>(samplerIds.size()); ++i)
    {
        auto const & samplerId = samplerIds[i];

        set(samplerId, i);
    }
}