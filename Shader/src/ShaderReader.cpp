#include <Shader/ShaderReader.hpp>

#include <fstream>

namespace ape
{

namespace
{

auto readShader(std::filesystem::path const & path)
    -> std::string
{
    auto shaderFile = std::ifstream{path, std::ifstream::ate | std::ifstream::binary};

    if (!shaderFile)
    {
        throw CouldNotOpenShaderFile{path};
    }

    auto const fileSize = static_cast<std::size_t>(shaderFile.tellg());

    shaderFile.seekg(0, std::ifstream::beg);

    auto data = std::string(fileSize, '\0');

    shaderFile.read(&data[0], fileSize);

    return data;
}

} // namespace ape

ShaderReader::ShaderReader(std::vector<std::filesystem::path> searchPaths)
    : searchPaths{std::move(searchPaths)}
{
}

auto ShaderReader::read(std::filesystem::path const & path) const
    -> std::string
{
    auto const absolutePath = resolveAbsolutePath(path);

    return readShader(absolutePath);
}

auto ShaderReader::resolveAbsolutePath(std::filesystem::path const & path) const
    -> std::filesystem::path
{
    if (path.is_absolute())
    {
        return path;
    }

    for (auto const & searchPath : searchPaths)
    {
        auto absolutePath = searchPath / path;

        if (std::filesystem::exists(absolutePath))
        {
            return absolutePath;
        }
    }

    throw CouldNotFindShaderFile{path};
}

} // namespace ape
