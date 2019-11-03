#pragma once

#include <Ape/GpuResource/Shader.hpp>

#include <string_view>

namespace ape
{

class VertexShader : public Shader
{

public:

    explicit VertexShader(std::string_view sourceCode)
        : Shader{Shader::Type::vertex, sourceCode}
    {
    }

};

} // namespace ape