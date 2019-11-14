#pragma once

#include <Glow/Shader/Shader.hpp>

#include <string_view>

namespace glow
{

class VertexShader : public Shader
{

public:

    explicit VertexShader(std::string_view sourceCode)
        : Shader{Shader::Type::vertex, sourceCode}
    {
    }

};

} // namespace glow
