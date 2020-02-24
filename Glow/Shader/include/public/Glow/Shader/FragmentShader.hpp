#pragma once

#include <Glow/Shader/Shader.hpp>

#include <string_view>

namespace glow
{

class FragmentShader : public Shader
{

public:

    static constexpr auto type = Shader::Type::fragment;

public:

    explicit FragmentShader(std::string_view const sourceCode)
        : FragmentShader{sourceCode, ""}
    {
    }

    FragmentShader(std::string_view const sourceCode, std::string_view const label)
        : Shader{type, sourceCode, label}
    {
    }

};

} // namespace glow
