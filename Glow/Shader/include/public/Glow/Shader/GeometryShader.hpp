#pragma once

#include <Glow/Shader/Shader.hpp>

#include <string_view>

namespace glow
{

class GeometryShader : public Shader
{

public:

    explicit GeometryShader(std::string_view sourceCode)
        : Shader{Shader::Type::geometry, sourceCode}
    {
    }

};

} // namespace glow
