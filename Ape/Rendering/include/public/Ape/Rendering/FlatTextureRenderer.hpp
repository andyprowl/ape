#pragma once

#include <Ape/Rendering/FlatQuad.hpp>

namespace ape
{

class FlatQuadShaderProgram;
class Texture;

class FlatTextureRenderer
{

public:

    explicit FlatTextureRenderer(FlatQuadShaderProgram & shader);

    auto render(Texture const & texture) const
        -> void;

private:

    auto drawQuad() const
        -> void;

private:

    FlatQuadShaderProgram * shader;

    FlatQuad planeShape;

};

} // namespace ape
