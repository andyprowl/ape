#include <Glow/Shader/ShaderBuilder.hpp>

#include <Glow/Shader/ShaderBuilderLogger.hpp>

#include <Basix/Range/Transform.hpp>

namespace glow
{

namespace
{

auto makeProgramShaderLabel(std::string_view const programLabel, std::string suffix)
    -> std::string
{
    if (programLabel.empty())
    {
        return "";
    }

    return std::string{programLabel} + std::move(suffix);
}

auto makeProgramVertexShaderLabel(std::string_view const programLabel)
    -> std::string
{
    return makeProgramShaderLabel(programLabel, ".Vertex");
}

auto makeProgramGeometryShaderLabel(std::string_view const programLabel)
    -> std::string
{
    return makeProgramShaderLabel(programLabel, ".Geometry");
}

auto makeProgramFragmentShaderLabel(std::string_view const programLabel)
    -> std::string
{
    return makeProgramShaderLabel(programLabel, ".Fragment");
}

} // unnamed namespace

ShaderBuilder::ShaderBuilder(logging::ShaderBuilderLogger & logger)
    : ShaderBuilder{{}, logger}
{
}

ShaderBuilder::ShaderBuilder(
    std::vector<std::filesystem::path> searchPaths,
    logging::ShaderBuilderLogger & logger)
    : logger{&logger}
    , reader{std::move(searchPaths)}
    , preprocessor{reader}
{
}

auto ShaderBuilder::getReader()
    -> ShaderReader &
{
    return reader;
}

auto ShaderBuilder::getReader() const
    -> ShaderReader const &
{
    return reader;
}

auto ShaderBuilder::getPreprocessor() const
    -> ShaderPreprocessor const &
{
    return preprocessor;
}

auto ShaderBuilder::buildProgram(
    VertexShaderPath const & vertexShaderPath,
    FragmentShaderPath const & fragmentShaderPath,
    std::string_view const label) const
    -> ShaderProgram
{
    using Entry = logging::ProgramBuildProgressLogEntry;

    logger->log(Entry{logging::ShaderBuilderOperationStatus::started});

    auto program = ShaderProgram{
        buildVertexShader(vertexShaderPath, makeProgramVertexShaderLabel(label)),
        buildFragmentShader(fragmentShaderPath, makeProgramFragmentShaderLabel(label))};

    logger->log(Entry{logging::ShaderBuilderOperationStatus::finished});

    return program;
}

auto ShaderBuilder::buildProgram(
    VertexShaderPath const & vertexShaderPath,
    GeometryShaderPath const & geometryShaderPath,
    FragmentShaderPath const & fragmentShaderPath,
    std::string_view const label) const
    -> ShaderProgram
{
    using Entry = logging::ProgramBuildProgressLogEntry;

    logger->log(Entry{logging::ShaderBuilderOperationStatus::started});

    auto program = ShaderProgram{
        buildVertexShader(vertexShaderPath, makeProgramVertexShaderLabel(label)),
        buildGeometryShader(geometryShaderPath, makeProgramGeometryShaderLabel(label)),
        buildFragmentShader(fragmentShaderPath, makeProgramFragmentShaderLabel(label))};

    logger->log(Entry{logging::ShaderBuilderOperationStatus::finished});

    return program;
}

auto ShaderBuilder::buildProgram(
    std::vector<VertexShaderPath> const & vertexShaderPaths,
    std::vector<GeometryShaderPath> const & geometryShaderPaths,
    std::vector<FragmentShaderPath> const & fragmentShaderPaths,
    std::string_view label) const
    -> ShaderProgram
{
    using Entry = logging::ProgramBuildProgressLogEntry;

    logger->log(Entry{logging::ShaderBuilderOperationStatus::started});

    auto vertexShaders = basix::transform(
        vertexShaderPaths,
        [&] (std::filesystem::path const & path)
    {
        return buildVertexShader(path, makeProgramVertexShaderLabel(label));
    });

    auto geometryShaders = basix::transform(
        geometryShaderPaths,
        [&] (std::filesystem::path const & path)
    {
        return buildGeometryShader(path, makeProgramGeometryShaderLabel(label));
    });

    auto fragmentShaders = basix::transform(
        fragmentShaderPaths,
        [&] (std::filesystem::path const & path)
    {
        return buildFragmentShader(path, makeProgramFragmentShaderLabel(label));
    });

    auto program = ShaderProgram{
        std::move(vertexShaders),
        std::move(geometryShaders),
        std::move(fragmentShaders)};

    logger->log(Entry{logging::ShaderBuilderOperationStatus::finished});

    return program;
}

auto ShaderBuilder::buildVertexShader(
    std::filesystem::path const & path,
    std::string_view const label) const
    -> VertexShader
{
    return buildShader<VertexShader>(path, label);
}

auto ShaderBuilder::buildGeometryShader(
    std::filesystem::path const & path,
    std::string_view const label) const
    -> GeometryShader
{
    return buildShader<GeometryShader>(path, label);
}

auto ShaderBuilder::buildFragmentShader(
    std::filesystem::path const & path,
    std::string_view const label) const
    -> FragmentShader
{
    return buildShader<FragmentShader>(path, label);
}

template<typename ShaderType>
auto ShaderBuilder::buildShader(
    std::filesystem::path const & path,
    std::string_view const label) const
    -> ShaderType
{
    using Entry = logging::ShaderBuildProgressLogEntry;

    logger->log(Entry{path, ShaderType::type, logging::ShaderBuilderOperationStatus::started});

    auto sourceCode = preprocessShader(path);

    auto shader = ShaderType{std::move(sourceCode), label};

    logger->log(Entry{path, ShaderType::type, logging::ShaderBuilderOperationStatus::finished});

    return shader;
}

auto ShaderBuilder::preprocessShader(std::filesystem::path const & path) const
    -> std::string
{
    auto sourceCode = reader.read(path);

    return preprocessor.preprocess(std::move(sourceCode));
}

} // namespace glow
