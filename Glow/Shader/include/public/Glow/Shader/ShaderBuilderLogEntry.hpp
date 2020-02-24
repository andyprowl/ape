#pragma once

#include <Glow/Shader/Shader.hpp>

#include <Basix/Logging/LogEntry.hpp>

#include <filesystem>

namespace glow::logging
{

inline namespace v1
{

enum class ShaderBuilderOperationStatus
{
    started,
    finished
};

class ShaderBuildProgressLogEntry : public basix::LogEntry
{

public:

    ShaderBuildProgressLogEntry(
        std::filesystem::path file,
        Shader::Type const type,
        ShaderBuilderOperationStatus const status)
        : LogEntry{basix::Severity::info}
        , file{std::move(file)}
        , type{type}
        , status{status}
    {
    }

public:

    std::filesystem::path file;

    Shader::Type type;

    ShaderBuilderOperationStatus status;

};

class ProgramBuildProgressLogEntry : public basix::LogEntry
{

public:

    explicit ProgramBuildProgressLogEntry(ShaderBuilderOperationStatus const status)
        : LogEntry{basix::Severity::info}
        , status{status}
    {
    }

public:

    ShaderBuilderOperationStatus status;

};

} // inline namespace v2

} // namespace glow::logging
