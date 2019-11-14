#pragma once

#include <Glow/Shader/Shader.hpp>

#include <string_view>

namespace glow
{

class FragmentShader : public Shader
{

public:

    explicit FragmentShader(std::string_view sourceCode)
        : Shader{Shader::Type::fragment, sourceCode}
    {
    }

};

} // namespace glow
