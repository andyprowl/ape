#pragma once

#include <Glow/Shader/Shader.hpp>

#include <filesystem>

namespace glow
{

template<Shader::Type ShaderType>
class ShaderPath : public std::filesystem::path
{

public:

    using path::path;

};

using VertexShaderPath = ShaderPath<Shader::Type::vertex>;

using GeometryShaderPath = ShaderPath<Shader::Type::geometry>;

using FragmentShaderPath = ShaderPath<Shader::Type::fragment>;

} // namespace glow
