#include <Rave/RaveCore/RaveSkyboxCollectionReader.hpp>

namespace rave
{

auto RaveSkyboxCollectionReader::read() const
    -> ape::SkyboxCollection
{
    auto const skyboxFolder = std::filesystem::path{resourceFolder} / "skyboxes";

    return reader.read(skyboxFolder);
}

} // namespace rave
