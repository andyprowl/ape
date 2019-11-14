#pragma once

#include <filesystem>
#include <optional>

namespace basix
{

class FileFinder
{

public:
    
    FileFinder() = default;

    explicit FileFinder(std::vector<std::filesystem::path> searchPaths);
    
    auto getSearchPaths() const
        -> std::vector<std::filesystem::path>;

    auto findExistingPath(std::filesystem::path const & path) const
        -> std::optional<std::filesystem::path>;

private:

    auto findFromAbsolutePath(std::filesystem::path const & path) const
        -> std::optional<std::filesystem::path>;

    auto findFromRelativePath(std::filesystem::path const & path) const
        -> std::optional<std::filesystem::path>;

private:

    std::vector<std::filesystem::path> searchPaths;

};

} // namespace basix
