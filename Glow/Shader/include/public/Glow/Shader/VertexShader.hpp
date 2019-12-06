#pragma once

#include <Glow/Shader/Shader.hpp>

#include <string_view>

namespace glow
{

class VertexShader : public Shader
{

public:

    explicit VertexShader(std::string_view const sourceCode)
        : VertexShader{sourceCode, ""}
    {
    }

    VertexShader(std::string_view const sourceCode, std::string_view const label)
        : Shader{Shader::Type::vertex, sourceCode, label}
    {
    }

};

} // namespace glow
