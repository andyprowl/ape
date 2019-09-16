#pragma once

#include <glm/mat4x4.hpp>

class Transformer;

class Transformable
{

public:

    Transformable();

    explicit Transformable(Transformer * transformer);

    virtual ~Transformable() = default;

    auto getTransformer() const
        -> Transformer *;

    auto setTransformer(Transformer & newTransformer)
        -> void;

    auto getContextTransformation() const
        -> glm::mat4;

protected:

    enum class BaseTransformationKind
    {
        local,
        context
    };

protected:

    Transformable(Transformable const & rhs) = default;

    Transformable(Transformable && rhs) noexcept = default;

    auto operator = (Transformable const & rhs)
        -> Transformable & = default;

    auto operator = (Transformable && rhs) noexcept
        -> Transformable & = default;

private:

    friend class Transformer;

private:

    // To be invoked by the parent Transformer.
    virtual auto onContextTransformationChanged(glm::mat4 const & contextTransformation)
        -> void = 0;

private:

    Transformer * transformer;

};