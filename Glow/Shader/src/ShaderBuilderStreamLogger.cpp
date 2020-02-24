#include <Glow/Shader/ShaderBuilderStreamLogger.hpp>

#include <cassert>
#include <string>

namespace glow::logging
{

namespace
{

auto formatBuildOperationStatus(ShaderBuilderOperationStatus const status)
    -> std::string
{
    switch (status)
    {
        case ShaderBuilderOperationStatus::started:
        {
            return "started";
        }

        case ShaderBuilderOperationStatus::finished:
        {
            return "finished";
        }

        default:
        {
            assert(false);

            return "<unrecognized-phase>";
        }
    }
}

auto formatShaderType(Shader::Type const type)
    -> std::string
{
    switch (type)
    {
        case Shader::Type::vertex:
        {
            return "Vertex";
        }

        case Shader::Type::geometry:
        {
            return "Geometry";
        }

        case Shader::Type::fragment:
        {
            return "Fragment";
        }

        default:
        {
            assert(false);

            return "Unknown";
        }
    }
}

} // unnamed namespace

ShaderBuilderStreamLogger::ShaderBuilderStreamLogger(std::ostream & os)
    : os{&os}
{
}

// virtual (from ShaderBuilderLogger)
auto ShaderBuilderStreamLogger::log(ShaderBuildProgressLogEntry const & entry)
    -> void
{
    auto formattedPhase = formatBuildOperationStatus(entry.status);

    auto formattedShaderType = formatShaderType(entry.type);

    *os << " - "
        << std::move(formattedShaderType)
        << " shader '" 
        << entry.file 
        << "' compilation "
        << std::move(formattedPhase)
        << "\n";
}

// virtual (from ShaderBuilderLogger)
auto ShaderBuilderStreamLogger::log(ProgramBuildProgressLogEntry const & entry)
    -> void
{
    auto formattedPhase = formatBuildOperationStatus(entry.status);

    *os << "Shader program build " << std::move(formattedPhase) << "\n";
}

} // namespace glow::logging
