#pragma once

#include "AssetRepository.hpp"

class AssetBuilder
{

public:

    auto build() const
        -> AssetRepository;

};
