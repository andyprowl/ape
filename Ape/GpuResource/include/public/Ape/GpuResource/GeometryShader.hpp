#pragma once

#include <Ape/GpuResource/Shader.hpp>

#include <string_view>

namespace ape
{

class GeometryShader : public Shader
{

public:

    explicit GeometryShader(std::string_view sourceCode)
        : Shader{Shader::Type::geometry, sourceCode}
    {
    }

};

} // namespace ape
