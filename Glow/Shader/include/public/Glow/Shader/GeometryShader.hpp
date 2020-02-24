#pragma once

#include <Glow/Shader/Shader.hpp>

#include <string_view>

namespace glow
{

class GeometryShader : public Shader
{

public:

    static constexpr auto type = Shader::Type::geometry;

public:

    explicit GeometryShader(std::string_view sourceCode)
        : GeometryShader{sourceCode, ""}
    {
    }

    GeometryShader(std::string_view const sourceCode, std::string_view const label)
        : Shader{type, sourceCode, label}
    {
    }

};

} // namespace glow
