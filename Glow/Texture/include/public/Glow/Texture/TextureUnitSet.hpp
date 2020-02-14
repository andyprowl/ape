#pragma once

#include <Glow/Texture/TextureUnit.hpp>

#include <vector>

namespace glow
{

class TextureUnitSet
{

public:

    TextureUnitSet();

    auto getUnit(int index)
        -> TextureUnit &;

    auto operator [] (int index)
        -> TextureUnit &;

private:

    std::vector<TextureUnit> units;

};

inline auto textureUnits = TextureUnitSet{};

} // namespace glow
