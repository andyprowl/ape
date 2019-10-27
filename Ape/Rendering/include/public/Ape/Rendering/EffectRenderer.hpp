#pragma once

#include <Ape/Rendering/FlatQuad.hpp>

namespace ape
{

class EffectSelector;
class EffectShaderProgram;
class Texture;

class EffectRenderer
{

public:

    explicit EffectRenderer(EffectSelector & selector);

    auto render(Texture const & texture) const
        -> void;

private:

    auto renderWithEffect(Texture const & texture, EffectShaderProgram & effect) const
        -> void;

    auto drawQuad() const
        -> void;

private:

    EffectSelector * selector;

    FlatQuad planeShape;

};

} // namespace ape
