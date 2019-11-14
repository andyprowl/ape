#include <Glow/Shader/ShaderProgram.hpp>

#include <glad/glad.h>

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>

namespace glow
{

namespace
{

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

ShaderProgram::ShaderProgram(VertexShader vertexShader, FragmentShader fragmentShader)
    : ShaderProgram{std::move(vertexShader), std::nullopt, std::move(fragmentShader)}
{
}

ShaderProgram::ShaderProgram(
    VertexShader vertexShader,
    std::optional<GeometryShader> geometryShader,
    FragmentShader fragmentShader)
    : resource{glCreateProgram(), glDeleteProgram}
    , vertexShader{std::move(vertexShader)}
    , geometryShader{std::move(geometryShader)}
    , fragmentShader{std::move(fragmentShader)}
{
    link();
}

auto ShaderProgram::bind() const
    -> void
{
    auto const id = resource.get();

    glUseProgram(id);

    assert(glGetError() == GL_NO_ERROR);
}

auto ShaderProgram::unbind() const
    -> void
{
    glUseProgram(0);
}

auto ShaderProgram::getVertexShader() const
    -> VertexShader const &
{
    return vertexShader;
}

auto ShaderProgram::getGeometryShader() const
    -> std::optional<GeometryShader> const &
{
    return geometryShader;
}

auto ShaderProgram::getFragmentShader() const
    -> FragmentShader const &
{
    return fragmentShader;
}

auto ShaderProgram::link()
    -> void
{
    auto const id = resource.get();

    glAttachShader(id, vertexShader.getId());

    if (geometryShader.has_value())
    {
        glAttachShader(id, geometryShader->getId());
    }

    glAttachShader(id, fragmentShader.getId());

    glLinkProgram(id);

    checkShaderLinkingOutcome(id);
}

auto ShaderProgram::getUniformLocation(std::string_view name) const
    -> int
{
    auto const id = resource.get();

    auto const location = glGetUniformLocation(id, name.data());

    if (location < 0)
    {
        throw UniformNotFound{std::string{name}};
    }

    return location;
}

} // namespace glow
