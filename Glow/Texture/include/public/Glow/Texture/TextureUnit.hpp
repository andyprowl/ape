#pragma once

namespace glow
{

class Texture;
class CubeTexture;

class TextureUnit
{

public:

    explicit TextureUnit(int unitIndex);

    auto getIndex() const
        -> int;

    auto activate()
        -> void;

    auto setTexture(Texture const *texture)
        -> void;

    auto setTexture(CubeTexture const * texture)
        -> void;

private:

    template<typename TextureType>
    auto setTexture(TextureType const * texture, TextureType const * & activeTexture)
        -> void;

    template<typename TextureType>
    auto resetActiveTexture(TextureType const * & activeTexture)
        -> void;

private:

    int unitIndex;

    Texture const * activeTexture2d;

    CubeTexture const * activeCubeTexture;

};

} // namespace glow
