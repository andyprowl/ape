#pragma once

#include <Ape/Shader/Shader.hpp>

#include <string_view>

namespace ape
{

class FragmentShader : public Shader
{

public:

    explicit FragmentShader(std::string_view sourceCode)
        : Shader{Shader::Type::fragment, sourceCode}
    {
    }

};

} // namespace ape
