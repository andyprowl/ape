#include <Shader/Shader.hpp>

#include <Shader/ShaderReader.hpp>

#include <glad/glad.h>

#include <array>

namespace ape
{

namespace
{

auto checkShaderCompilationOutcome(int const shaderId)
    -> void
{
    auto success = int{};

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if (success)
    {
        return;
    }
    
    auto infoLog = std::array<char, 512>{'\0'};

    glGetShaderInfoLog(shaderId, sizeof(infoLog), nullptr, infoLog.data());

    throw CouldNotCompileShader{infoLog.data()};
}

} // unnamed namespace

Shader::Shader(Shader && rhs) noexcept
    : id{rhs.id}
{
    rhs.id = 0;
}

auto Shader::operator = (Shader && rhs) noexcept
    -> Shader &
{
    destroy();

    id = rhs.id;

    rhs.id = 0;

    return *this;
}

Shader::~Shader()
{
    destroy();
}

auto Shader::getId() const
    -> unsigned int
{
    return id;
}

auto Shader::getSourceCode() const
    -> std::string
{
    return sourceCode;
}

auto Shader::recompile(std::string newSourceCode)
    -> void
{
    compile(newSourceCode);

    sourceCode = std::move(newSourceCode);
}

auto Shader::release()
    -> unsigned int
{
    auto const shaderId = id;

    id = 0;

    return shaderId;
}

Shader::Shader(unsigned int id, std::string sourceCode)
    : id{id}
    , sourceCode{std::move(sourceCode)}
{
    compile(this->sourceCode);
}

auto Shader::compile(std::string const & newSourceCode)
    -> void
{
    auto const code = newSourceCode.data();

    glShaderSource(id, 1, &code, nullptr);

    glCompileShader(id);

    checkShaderCompilationOutcome(id);
}

auto Shader::destroy()
    -> void
{
    if (id != 0)
    {
        glDeleteShader(id);
    }
}

} // namespace ape
