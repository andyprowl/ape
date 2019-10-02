#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>

namespace ape
{

class CouldNotFindShaderFile : public std::logic_error
{

public:

    explicit CouldNotFindShaderFile(std::filesystem::path const & path)
        : logic_error{"The shader file '" + path.string() + "' was not found"}
    {
    }

};

class CouldNotOpenShaderFile : public std::logic_error
{

public:

    explicit CouldNotOpenShaderFile(std::filesystem::path const & path)
        : logic_error{"Failed to open the shader file '" + path.string() + "'"}
    {
    }

};

class ShaderReader
{

public:

    ShaderReader() = default;

    explicit ShaderReader(std::vector<std::filesystem::path> searchPaths);

    auto read(std::filesystem::path const & path) const
        -> std::string;

private:

    auto resolveAbsolutePath(std::filesystem::path const & path) const
        -> std::filesystem::path;

private:

    std::vector<std::filesystem::path> searchPaths;

};

} // namespace ape
