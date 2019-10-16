#include <Ape/GpuResource/FileFinder.hpp>

namespace ape
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

auto FileFinder::findExistingFile(std::filesystem::path const & path) const
    -> std::optional<std::filesystem::path>
{
    if (path.is_absolute())
    {
        return findFileFromAbsolutePath(path);
    }
    else
    {
        return findFileFromRelativePath(path);
    }
}

auto FileFinder::findFileFromAbsolutePath(std::filesystem::path const & path) const
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

auto FileFinder::findFileFromRelativePath(std::filesystem::path const & path) const
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

} // namespace ape
