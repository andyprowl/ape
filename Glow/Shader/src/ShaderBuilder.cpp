#include <Glow/Shader/ShaderBuilder.hpp>

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

ShaderBuilder::ShaderBuilder()
    : ShaderBuilder{{}}
{
}

ShaderBuilder::ShaderBuilder(std::vector<std::filesystem::path> searchPaths)
    : reader{std::move(searchPaths)}
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
    return {
        buildVertexShader(vertexShaderPath, makeProgramVertexShaderLabel(label)),
        buildFragmentShader(fragmentShaderPath, makeProgramFragmentShaderLabel(label))};
}

auto ShaderBuilder::buildProgram(
    VertexShaderPath const & vertexShaderPath,
    GeometryShaderPath const & geometryShaderPath,
    FragmentShaderPath const & fragmentShaderPath,
    std::string_view const label) const
    -> ShaderProgram
{
    return {
        buildVertexShader(vertexShaderPath, makeProgramVertexShaderLabel(label)),
        buildGeometryShader(geometryShaderPath, makeProgramGeometryShaderLabel(label)),
        buildFragmentShader(fragmentShaderPath, makeProgramFragmentShaderLabel(label))};
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
    auto sourceCode = preprocessShader(path);

    return ShaderType{std::move(sourceCode), label};
}

auto ShaderBuilder::preprocessShader(std::filesystem::path const & path) const
    -> std::string
{
    auto sourceCode = reader.read(path);

    return preprocessor.preprocess(std::move(sourceCode));
}

} // namespace glow
