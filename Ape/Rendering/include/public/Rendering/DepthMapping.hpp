#pragma once

#include <Rendering/DepthMap.hpp>

#include <vector>

namespace ape
{

class DepthMapping
{

public:

    std::vector<DepthMap> directionalMapping;

    std::vector<DepthMap> spotMapping;

    std::vector<DepthMap> pointMapping;

};

} // namespace ape
