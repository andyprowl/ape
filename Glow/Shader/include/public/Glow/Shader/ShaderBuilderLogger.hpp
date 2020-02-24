#pragma once

#include <Glow/Shader/Shader.hpp>
#include <Glow/Shader/ShaderBuilderLogEntry.hpp>

#include <filesystem>

namespace glow::logging
{

class ShaderBuilderLogger
{

public:

    virtual ~ShaderBuilderLogger() = default;

    virtual auto log(ShaderBuildProgressLogEntry const & entry)
        -> void = 0;

    virtual auto log(ProgramBuildProgressLogEntry const & entry)
        -> void = 0;

};

} // namespace glow::logging
