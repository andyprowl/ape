#pragma once

namespace glow
{

class Texture2d;
class TextureCube;

class TextureUnit
{

public:

    explicit TextureUnit(int unitIndex);

    auto getIndex() const
        -> int;

    auto activate()
        -> void;

    auto setTexture(Texture2d const *texture)
        -> void;

    auto setTexture(TextureCube const * texture)
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

    Texture2d const * activeTexture2d;

    TextureCube const * activeCubeTexture;

};

} // namespace glow
