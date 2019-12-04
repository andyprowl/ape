#pragma once

#include <glm/vec3.hpp>

#include <string_view>

namespace ape
{

class Light;

class LightColorUpdaterOverlay
{

public:

    auto updateLightColor(Light & light)
        -> void;

private:

    auto updateColor(std::string_view const label, glm::vec3 & color)
        -> bool;

};

} // namespace ape
