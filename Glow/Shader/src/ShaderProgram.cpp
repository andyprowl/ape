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

// Necessary because initializer lists do not play well with move-only types.
template<typename T>
auto makeSingleElementVector(T && value)
    -> std::vector<T>
{
    auto v = std::vector<T>{};

    v.push_back(std::move(value));

    return v;
}

} // unnamed namespace

ShaderProgram::ShaderProgram(
    VertexShader vertexShader,
    FragmentShader fragmentShader,
    std::string_view const label)
    : ShaderProgram{std::move(vertexShader), std::nullopt, std::move(fragmentShader), label}
{
}

ShaderProgram::ShaderProgram(
    VertexShader vertexShader,
    std::optional<GeometryShader> geometryShader,
    FragmentShader fragmentShader,
    std::string_view const label)
    : ShaderProgram{
        makeSingleElementVector(std::move(vertexShader)),
        geometryShader.has_value()
            ? makeSingleElementVector(std::move(*geometryShader))
            : std::vector<GeometryShader>{},
        makeSingleElementVector(std::move(fragmentShader)),
        label}
{
}

ShaderProgram::ShaderProgram(
    std::vector<VertexShader> vertexShaders,
    std::vector<GeometryShader> geometryShaders,
    std::vector<FragmentShader> fragmentShaders,
    std::string_view label)
    : resource{glCreateProgram(), glDeleteProgram}
    , vertexShaders{std::move(vertexShaders)}
    , geometryShaders{std::move(geometryShaders)}
    , fragmentShaders{std::move(fragmentShaders)}
{
    link();

    setLabel(label);
}

auto ShaderProgram::bind() const
    -> void
{
    auto const id = getId();

    glUseProgram(id);

    assert(glGetError() == GL_NO_ERROR);
}

auto ShaderProgram::unbind() const
    -> void
{
    glUseProgram(0);
}

auto ShaderProgram::getId() const
    -> GpuResource::Id
{
    return resource.get();
}

auto ShaderProgram::getVertexShaders() const
    -> std::vector<VertexShader> const &
{
    return vertexShaders;
}

auto ShaderProgram::getGeometryShaders() const
    -> std::vector<GeometryShader> const &
{
    return geometryShaders;
}

auto ShaderProgram::getFragmentShaders() const
    -> std::vector<FragmentShader> const &
{
    return fragmentShaders;
}

auto ShaderProgram::setLabel(std::string_view const label)
    -> void
{
    glObjectLabel(GL_PROGRAM, getId(), static_cast<GLsizei>(label.size()), label.data());
}

auto ShaderProgram::link()
    -> void
{
    auto const id = resource.get();

    for (auto const & vertexShader : vertexShaders)
    {
        glAttachShader(id, vertexShader.getId());
    }

    for (auto const & geometryShader : geometryShaders)
    {
        glAttachShader(id, geometryShader.getId());
    }

    for (auto const & fragmentShader : fragmentShaders)
    {
        glAttachShader(id, fragmentShader.getId());
    }

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
