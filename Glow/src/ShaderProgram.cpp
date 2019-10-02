#include <Glow/ShaderProgram.hpp>

#include <CompilerSupport/CompilerWarnings.hpp>

#include <glad/glad.h>

// Non-standard extensions used in GLM
disableCompilerWarnings()
#include <glm/gtc/type_ptr.hpp>
restoreCompilerWarnings()

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>

namespace ape
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
    : id{glCreateProgram()}
    , vertexShader{std::move(vertexShader)}
    , geometryShader{std::move(geometryShader)}
    , fragmentShader{std::move(fragmentShader)}
{
    link();
}

ShaderProgram::ShaderProgram(ShaderProgram && rhs) noexcept
    : id{rhs.id}
    , vertexShader{std::move(rhs.vertexShader)}
    , geometryShader{std::move(rhs.geometryShader)}
    , fragmentShader{std::move(rhs.fragmentShader)}
{
    rhs.id = 0;
}

auto ShaderProgram::operator = (ShaderProgram && rhs) noexcept
    -> ShaderProgram &
{
    destroy();

    id = rhs.id;

    rhs.id = 0;

    return *this;
}

ShaderProgram::~ShaderProgram()
{
    destroy();
}

auto ShaderProgram::use() const
    -> void
{
    glUseProgram(id);

    assert(glGetError() == GL_NO_ERROR);
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
    glAttachShader(id, vertexShader.getId());

    if (geometryShader.has_value())
    {
        glAttachShader(id, geometryShader->getId());
    }

    glAttachShader(id, fragmentShader.getId());

    glLinkProgram(id);

    checkShaderLinkingOutcome(id);
}

auto ShaderProgram::destroy()
    -> void
{
    if (id != 0)
    {
        glDeleteProgram(id);
    }
}

auto ShaderProgram::getUniformLocation(std::string const & name) const
    -> int
{
    auto const location = glGetUniformLocation(id, name.c_str());

    if (location < 0)
    {
        throw UniformNotFound{name};
    }

    return location;
}

} // namespace ape
