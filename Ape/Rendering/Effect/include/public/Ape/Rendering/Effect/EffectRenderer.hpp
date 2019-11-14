#pragma once

#include <Ape/Rendering/Effect/PlainQuad.hpp>

namespace glow
{

class Texture;

} // namespace glow

namespace ape
{

class EffectSelector;
class EffectShaderProgram;

class EffectRenderer
{

public:

    explicit EffectRenderer(EffectSelector & selector);

    auto render(glow::Texture const & texture) const
        -> void;

private:

    auto renderWithEffect(glow::Texture const & texture, EffectShaderProgram & effect) const
        -> void;

    auto drawQuad() const
        -> void;

private:

    EffectSelector * selector;

    PlainQuad planeShape;

};

} // namespace ape
