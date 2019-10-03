#pragma once

#include <GpuResource/Shader.hpp>

namespace ape
{

class FragmentShader : public Shader
{

public:

    explicit FragmentShader(std::string const & sourceCode)
        : Shader{Shader::Type::fragment, sourceCode}
    {
    }

};

} // namespace ape
