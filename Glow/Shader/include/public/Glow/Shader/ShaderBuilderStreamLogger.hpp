#pragma once

#include <Glow/Shader/ShaderBuilderLogger.hpp>

#include <ostream>

namespace glow::logging
{

class ShaderBuilderStreamLogger : public ShaderBuilderLogger
{

public:

    explicit ShaderBuilderStreamLogger(std::ostream & os);

    // virtual (from ShaderBuilderLogger)
    auto log(ShaderBuildProgressLogEntry const & entry)
        -> void override;

    // virtual (from ShaderBuilderLogger)
    auto log(ProgramBuildProgressLogEntry const & entry)
        -> void override;

private:

    std::ostream * os;

};

} // namespace glow::logging
