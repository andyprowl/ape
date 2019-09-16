#include "Transformable.hpp"

#include "Transformer.hpp"

Transformable::Transformable()
    : Transformable{glm::mat4{1.0f}}
{
}

Transformable::Transformable(glm::mat4 const & localTransformation)
    : Transformable{nullptr, localTransformation}
{
}

Transformable::Transformable(Transformer * const transformer, glm::mat4 const & localTransformation)
    : transformer{transformer}
    , localTransformation{localTransformation}
    , globalTransformation{getContextTransformation() * localTransformation}
{
}

auto Transformable::getTransformer() const
    -> Transformer *
{
    return transformer;
}

auto Transformable::getLocalTransformation() const
    -> glm::mat4
{
    return localTransformation;
}

auto Transformable::getGlobalTransformation() const
    -> glm::mat4
{
    return globalTransformation;
}

auto Transformable::setTransformer(Transformer & newTransformer)
    -> void
{
    if (transformer != nullptr)
    {
        transformer->removeSubject(*this);
    }

    transformer = &newTransformer;

    if (transformer != nullptr)
    {
        transformer->addSubject(*this);
    }

    auto const contextTransformation = getContextTransformation();

    onContextTransformationChanged(contextTransformation);
}

auto Transformable::setLocalTransformation(glm::mat4 const & newTransformation)
    -> void
{
    localTransformation = newTransformation;

    auto const contextTransformation = getContextTransformation();

    globalTransformation = contextTransformation * localTransformation;

    onTransformationChanged(BaseTransformationKind::local);
}

// To be invoked by the parent Transformer.
auto Transformable::onContextTransformationChanged(glm::mat4 const & contextTransformation)
    -> void
{
    globalTransformation = contextTransformation * localTransformation;

    onTransformationChanged(BaseTransformationKind::context);
}

auto Transformable::getContextTransformation() const
    -> glm::mat4
{
    if (transformer == nullptr)
    {
        return glm::mat4{1.0};
    }

    return transformer->getGlobalTransformation();
}
