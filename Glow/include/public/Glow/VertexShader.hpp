#pragma once

#include <Glow/Shader.hpp>

namespace ape
{

class VertexShader : public Shader
{

public:

    explicit VertexShader(std::string const & sourceCode)
        : Shader{Shader::Type::vertex, sourceCode}
    {
    }

};

} // namespace ape
