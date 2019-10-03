#include <GpuResource/Shader.hpp>


#include <GpuResource/ShaderReader.hpp>

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

auto Shader::getId() const
    -> unsigned int
{
    return resource.get();
}

auto Shader::compile(std::string const & sourceCode)
    -> void
{
    auto const id = resource.get();

    auto const code = sourceCode.data();

    glShaderSource(id, 1, &code, nullptr);

    glCompileShader(id);

    checkShaderCompilationOutcome(id);

    assert(getSourceCode() == sourceCode);
}

auto Shader::getSourceCode() const
    -> std::string
{
    auto const id = resource.get();

    auto sourceLength = 0;

    glGetShaderiv(id, GL_SHADER_SOURCE_LENGTH, &sourceLength);
 	
    auto source = std::string(static_cast<std::size_t>(sourceLength) - 1u, '\0');

    glGetShaderSource(id, sourceLength, nullptr, source.data());

    return source;
}

auto Shader::getType() const
    -> Type
{
    auto const id = resource.get();

    auto type = 0;

    glGetShaderiv(id, GL_SHADER_TYPE, &type);

    return convertFromOpenGLShaderType(static_cast<GLenum>(type));
}

auto Shader::release()
    -> unsigned int
{
    return resource.release();
}

Shader::Shader(Type const type, std::string const & sourceCode)
    : resource{createShaderResource(type)}
{
    compile(sourceCode);

    assert(getType() == type);
}

auto Shader::createShaderResource(Type const type) const
    -> GpuResource
{
    auto const shaderType = convertToOpenGLShaderType(type);

    auto const id = glCreateShader(shaderType);

    return GpuResource{id, glDeleteShader};
}

} // namespace ape
