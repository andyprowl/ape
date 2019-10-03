#include <GpuResource/ShaderBuilder.hpp>

namespace ape
{

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
    std::filesystem::path const & vertexShaderPath,
    std::filesystem::path const & fragmentShaderPath) const
    -> ShaderProgram
{
    return {
        buildVertexShader(vertexShaderPath),
        buildFragmentShader(fragmentShaderPath)};
}

auto ShaderBuilder::buildProgram(
    std::filesystem::path const & vertexShaderPath,
    std::filesystem::path const & geometryShaderPath,
    std::filesystem::path const & fragmentShaderPath) const
    -> ShaderProgram
{
    return {
        buildVertexShader(vertexShaderPath),
        buildGeometryShader(geometryShaderPath),
        buildFragmentShader(fragmentShaderPath)};
}

auto ShaderBuilder::buildVertexShader(std::filesystem::path const & path) const
    -> VertexShader
{
    return buildShader<VertexShader>(path);
}

auto ShaderBuilder::buildGeometryShader(std::filesystem::path const & path) const
    -> GeometryShader
{
    return buildShader<GeometryShader>(path);
}

auto ShaderBuilder::buildFragmentShader(std::filesystem::path const & path) const
    -> FragmentShader
{
    return buildShader<FragmentShader>(path);
}

template<typename ShaderType>
auto ShaderBuilder::buildShader(std::filesystem::path const & path) const
    -> ShaderType
{
    auto sourceCode = preprocessShader(path);

    return ShaderType{std::move(sourceCode)};
}

auto ShaderBuilder::preprocessShader(std::filesystem::path const & path) const
    -> std::string
{
    auto sourceCode = reader.read(path);

    return preprocessor.preprocess(std::move(sourceCode));
}

} // namespace ape
