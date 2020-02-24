#pragma once

#include <Glow/Shader/Shader.hpp>

#include <string_view>

namespace glow
{

class VertexShader : public Shader
{

public:

    static constexpr auto type = Shader::Type::vertex;

public:

    explicit VertexShader(std::string_view const sourceCode)
        : VertexShader{sourceCode, ""}
    {
    }

    VertexShader(std::string_view const sourceCode, std::string_view const label)
        : Shader{type, sourceCode, label}
    {
    }

};

} // namespace glow
