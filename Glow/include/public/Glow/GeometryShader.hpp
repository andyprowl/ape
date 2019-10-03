#pragma once

#include <Glow/Shader.hpp>

namespace ape
{

class GeometryShader : public Shader
{

public:

    explicit GeometryShader(std::string const & sourceCode)
        : Shader{Shader::Type::geometry, sourceCode}
    {
    }

};

} // namespace ape
