#include <Glow/Shader/ShaderReader.hpp>

#include <fstream>

namespace glow
{

namespace
{

auto readShaderCode(std::filesystem::path const & path)
    -> std::string
{
    auto shaderFile = std::ifstream{path, std::ifstream::ate | std::ifstream::binary};

    if (not shaderFile)
    {
        throw CouldNotOpenShaderFile{path};
    }

    auto const fileSize = static_cast<std::size_t>(shaderFile.tellg());

    shaderFile.seekg(0, std::ifstream::beg);

    auto data = std::string(fileSize, '\0');

    shaderFile.read(&data[0], fileSize);

    return data;
}

} // namespace glow

ShaderReader::ShaderReader(std::vector<std::filesystem::path> searchPaths)
    : fileFinder{std::move(searchPaths)}
{
}

auto ShaderReader::read(std::filesystem::path const & path) const
    -> std::string
{
    auto const absolutePath = resolveToPathOfExistingFile(path);

    return readShaderCode(absolutePath);
}

auto ShaderReader::getSearchPaths() const
    -> std::vector<std::filesystem::path>
{
    return fileFinder.getSearchPaths();
}

auto ShaderReader::resolveToPathOfExistingFile(std::filesystem::path const & path) const
    -> std::filesystem::path
{
    auto existingFilePath = fileFinder.findExistingPath(path);

    if (not existingFilePath.has_value())
    {
        throw CouldNotFindShaderFile{path};
    }

    return *existingFilePath;
}

} // namespace glow
