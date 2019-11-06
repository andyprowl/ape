#include "Catch.hpp"

#include <Stb/stb_image.h>

#include <filesystem>

namespace
{

auto loadImagePixels(std::filesystem::path const & path)
    -> stbi_uc *
{
    auto width = 0;

    auto height = 0;

    auto numOfChannels = 0;

    return stbi_load(path.string().c_str(), &width, &height, &numOfChannels, 0);
}

} // unnamed namespace

TEST_CASE("Verify expected linear image value of gray", "[srgb]")
{
    auto const imagePath = std::filesystem::path{resourceFolder} / "Images/Linear.png";

    auto const bytes = loadImagePixels(imagePath);

    REQUIRE(bytes != nullptr);

    auto const pixelColor = (bytes[0] & 0x0ff);

    REQUIRE(pixelColor == 55);

    stbi_image_free(bytes);
}

TEST_CASE("Verify expected perceptual image value of gray", "[srgb]")
{
    auto const imagePath = std::filesystem::path{resourceFolder} / "Images/Perceptual.png";

    auto const bytes = loadImagePixels(imagePath);

    REQUIRE(bytes != nullptr);

    auto const pixelColor = (bytes[0] & 0x0ff);

    REQUIRE(pixelColor == 128);

    stbi_image_free(bytes);
}
