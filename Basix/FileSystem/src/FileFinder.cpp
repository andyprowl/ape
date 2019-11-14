#include <Basix/Filesystem/FileFinder.hpp>

namespace basix
{

FileFinder::FileFinder(std::vector<std::filesystem::path> searchPaths)
    : searchPaths{std::move(searchPaths)}
{
}

auto FileFinder::getSearchPaths() const
    -> std::vector<std::filesystem::path>
{
    return searchPaths;
}

auto FileFinder::findExistingPath(std::filesystem::path const & path) const
    -> std::optional<std::filesystem::path>
{
    if (path.is_absolute())
    {
        return findFromAbsolutePath(path);
    }
    else
    {
        return findFromRelativePath(path);
    }
}

auto FileFinder::findFromAbsolutePath(std::filesystem::path const & path) const
    -> std::optional<std::filesystem::path>
{
    if (std::filesystem::exists(path))
    {
        return path;
    }
    else
    {
        return std::nullopt;
    }
}

auto FileFinder::findFromRelativePath(std::filesystem::path const & path) const
    -> std::optional<std::filesystem::path>
{
    for (auto const & searchPath : searchPaths)
    {
        auto absolutePath = searchPath / path;

        if (std::filesystem::exists(absolutePath))
        {
            return absolutePath;
        }
    }

    return std::nullopt;
}

} // namespace basix
