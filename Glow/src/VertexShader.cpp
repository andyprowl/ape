#include <Glow/VertexShader.hpp>

#include <Glow/ShaderReader.hpp>

#include <glad/glad.h>

#include <array>

namespace ape
{

VertexShader::VertexShader(std::string sourceCode)
    : Shader{glCreateShader(GL_VERTEX_SHADER), std::move(sourceCode)}
{
}

} // namespace ape