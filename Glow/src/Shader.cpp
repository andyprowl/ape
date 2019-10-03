#include <Glow/Shader.hpp>


#include <Glow/ShaderReader.hpp>

#include <glad/glad.h>

#include <array>
#include <cassert>
#include <unordered_map>

namespace ape
{

namespace
{

auto const shaderTypeMap = std::unordered_map<Shader::Type, GLenum>{
    {Shader::Type::vertex, GL_VERTEX_SHADER},
    {Shader::Type::geometry, GL_GEOMETRY_SHADER},
    {Shader::Type::fragment, GL_FRAGMENT_SHADER}};

auto convertToOpenGLShaderType(Shader::Type const type)
    -> GLenum
{
    auto const it = shaderTypeMap.find(type);

    if (it == std::cend(shaderTypeMap))
    {
        assert(false);

        return 0;
    }

    return it->second;
}

auto convertFromOpenGLShaderType(GLenum const type)
    -> Shader::Type
{
    for (auto const & entry : shaderTypeMap)
    {
        if (entry.second == type)
        {
            return entry.first;
        }
    }

    assert(false);

    return Shader::Type::unknown;
}

auto generateNewShaderId(Shader::Type const type)
    -> unsigned int
{
    auto const shaderType = convertToOpenGLShaderType(type);

    return glCreateShader(shaderType);
}

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

auto Shader::compile(std::string const & sourceCode)
    -> void
{
    auto const code = sourceCode.data();

    glShaderSource(id, 1, &code, nullptr);

    glCompileShader(id);

    checkShaderCompilationOutcome(id);
}

auto Shader::getSourceCode() const
    -> std::string
{
    auto sourceLength = 0;

    glGetShaderiv(id, GL_SHADER_SOURCE_LENGTH, &sourceLength);
 	
    auto source = std::string(sourceLength, '\0');

    glGetShaderSource(id, sourceLength, nullptr, source.data());

    return source;
}

auto Shader::getType() const
    -> Type
{
    auto type = 0;

    glGetShaderiv(id, GL_SHADER_TYPE, &type);

    return convertFromOpenGLShaderType(static_cast<GLenum>(type));
}

auto Shader::release()
    -> unsigned int
{
    auto const shaderId = id;

    id = 0;

    return shaderId;
}

Shader::Shader(Type const type, std::string const & sourceCode)
    : id{generateNewShaderId(type)}
{
    compile(sourceCode);
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
