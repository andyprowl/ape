#pragma once

#include <Ape/Rendering/Effect/PlainQuad.hpp>

namespace glow
{

class Texture2d;

} // namespace glow

namespace ape
{

class EffectSelector;
class EffectShaderProgram;

class EffectRenderer
{

public:

    explicit EffectRenderer(EffectSelector & selector);

    auto render(glow::Texture2d const & texture) const
        -> void;

private:

    auto renderWithEffect(glow::Texture2d const & texture, EffectShaderProgram & effect) const
        -> void;

    auto drawQuad() const
        -> void;

private:

    EffectSelector * selector;

    PlainQuad planeShape;

};

} // namespace ape
