#include <Glow/Shader/Shader.hpp>

#include <Glow/Shader/ShaderReader.hpp>

#include <glad/glad.h>

#include <array>
#include <cassert>
#include <unordered_map>

namespace glow
{

namespace
{

auto const shaderTypeMap = std::unordered_map<Shader::Type, GLenum>{
    {Shader::Type::vertex, GL_VERTEX_SHADER},
    {Shader::Type::geometry, GL_GEOMETRY_SHADER},
    {Shader::Type::fragment, GL_FRAGMENT_SHADER}};

auto const shaderTypeFormatMap = std::unordered_map<Shader::Type, std::string>{
    {Shader::Type::vertex, "vertex"},
    {Shader::Type::geometry, "geometry"},
    {Shader::Type::fragment, "fragment"}};

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

auto checkShaderCompilationOutcome(int const shaderId, Shader::Type const shaderType)
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

    throw CouldNotCompileShader{infoLog.data(), shaderTypeFormatMap.at(shaderType)};
}

} // unnamed namespace

auto Shader::getId() const
    -> GpuResource::Id
{
    return resource.get();
}

auto Shader::compile(std::string_view sourceCode)
    -> void
{
    auto const id = getId();

    auto const code = sourceCode.data();

    glShaderSource(id, 1, &code, nullptr);

    glCompileShader(id);

    checkShaderCompilationOutcome(id, getType());

    assert(getSourceCode() == sourceCode);
}

auto Shader::getSourceCode() const
    -> std::string
{
    auto const id = getId();

    auto sourceLength = 0;

    glGetShaderiv(id, GL_SHADER_SOURCE_LENGTH, &sourceLength);
 	
    auto source = std::string(static_cast<std::size_t>(sourceLength) - 1u, '\0');

    glGetShaderSource(id, sourceLength, nullptr, source.data());

    return source;
}

auto Shader::getType() const
    -> Type
{
    auto const id = getId();

    auto type = 0;

    glGetShaderiv(id, GL_SHADER_TYPE, &type);

    return convertFromOpenGLShaderType(static_cast<GLenum>(type));
}

auto Shader::release()
    -> GpuResource::Id
{
    return resource.release();
}

auto Shader::setLabel(std::string_view const label)
    -> void
{
    glObjectLabel(GL_SHADER, getId(), static_cast<GLsizei>(label.size()), label.data());
}

Shader::Shader(Type const type, std::string_view const sourceCode, std::string_view const label)
    : resource{createShaderResource(type)}
{
    compile(sourceCode);

    assert(getType() == type);

    setLabel(label);
}

auto Shader::createShaderResource(Type const type) const
    -> GpuResource
{
    auto const shaderType = convertToOpenGLShaderType(type);

    auto const id = glCreateShader(shaderType);

    return GpuResource{id, glDeleteShader};
}

} // namespace glow
