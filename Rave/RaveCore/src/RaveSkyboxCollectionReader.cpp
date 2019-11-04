#include <Rave/RaveCore/RaveSkyboxCollectionReader.hpp>

#include <Ape/Rendering/SkyboxCollectionPopulator.hpp>

namespace rave
{

auto RaveSkyboxCollectionReader::read() const
    -> ape::SkyboxCollection
{
    auto collection = ape::SkyboxCollection{};

    auto const populator = ape::SkyboxCollectionPopulator{collection};

    auto const skyboxFolder = std::filesystem::path{resourceFolder} / "Skyboxes";

    populator.addAllSkyboxesInFolder(skyboxFolder);

    return collection;
}

} // namespace rave
