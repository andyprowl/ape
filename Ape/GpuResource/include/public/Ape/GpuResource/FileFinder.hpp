#pragma once

#include <filesystem>
#include <optional>

namespace ape
{

class FileFinder
{

public:
    
    FileFinder() = default;

    explicit FileFinder(std::vector<std::filesystem::path> searchPaths);
    
    auto getSearchPaths() const
        -> std::vector<std::filesystem::path>;

    auto findExistingFile(std::filesystem::path const & path) const
        -> std::optional<std::filesystem::path>;

private:

    auto findFileFromAbsolutePath(std::filesystem::path const & path) const
        -> std::optional<std::filesystem::path>;

    auto findFileFromRelativePath(std::filesystem::path const & path) const
        -> std::optional<std::filesystem::path>;

private:

    std::vector<std::filesystem::path> searchPaths;

};

} // namespace ape
