#include <Core/FragmentShader.hpp>

#include <Core/ShaderReader.hpp>

#include <glad/glad.h>

#include <array>

namespace ape
{

FragmentShader::FragmentShader(std::string sourceCode)
    : Shader{glCreateShader(GL_FRAGMENT_SHADER), std::move(sourceCode)}
{
}

} // namespace ape
