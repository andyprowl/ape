#include "Transformable.hpp"

#include "Transformer.hpp"

Transformable::Transformable()
    : Transformable{nullptr}
{
}

Transformable::Transformable(Transformer * const transformer)
    : transformer{transformer}
{
    if (transformer != nullptr)
    {
        transformer->addSubject(*this);
    }
}

auto Transformable::getTransformer() const
    -> Transformer *
{
    return transformer;
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

auto Transformable::getContextTransformation() const
    -> glm::mat4
{
    if (transformer == nullptr)
    {
        return glm::mat4{1.0};
    }

    return transformer->getGlobalTransformation();
}
