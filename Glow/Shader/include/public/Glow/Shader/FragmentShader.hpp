#pragma once

#include <Glow/Shader/Shader.hpp>

#include <string_view>

namespace glow
{

class FragmentShader : public Shader
{

public:

    explicit FragmentShader(std::string_view const sourceCode)
        : FragmentShader{sourceCode, ""}
    {
    }

    FragmentShader(std::string_view const sourceCode, std::string_view const label)
        : Shader{Shader::Type::fragment, sourceCode, label}
    {
    }

};

} // namespace glow
