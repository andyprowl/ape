#pragma once

#include <glm/mat4x4.hpp>

class Transformer;

class Transformable
{

public:

    Transformable();

    explicit Transformable(glm::mat4 const & localTransformation);

    Transformable(Transformer * transformer, glm::mat4 const & localTransformation);

    virtual ~Transformable() = default;

    auto getTransformer() const
        -> Transformer *;

    auto getLocalTransformation() const
        -> glm::mat4;

    auto getGlobalTransformation() const
        -> glm::mat4;

    auto setTransformer(Transformer & newTransformer)
        -> void;

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

    auto setLocalTransformation(glm::mat4 const & newTransformation)
        -> void;

private:

    friend class Transformer;

private:

    virtual auto onTransformationChanged(BaseTransformationKind const)
        -> void
    {
        // Default implementation intentionally a NOP.
    }

    // To be invoked by the parent Transformer.
    auto onContextTransformationChanged(glm::mat4 const & contextTransformation)
        -> void;

    auto getContextTransformation() const
        -> glm::mat4;

private:

    Transformer * transformer;

    glm::mat4 localTransformation;

    glm::mat4 globalTransformation;

};