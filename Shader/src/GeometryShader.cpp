#include <Shader/GeometryShader.hpp>

#include <Shader/ShaderReader.hpp>

#include <glad/glad.h>

#include <array>

namespace ape
{

GeometryShader::GeometryShader(std::string sourceCode)
    : Shader{glCreateShader(GL_GEOMETRY_SHADER), std::move(sourceCode)}
{
}

} // namespace ape
