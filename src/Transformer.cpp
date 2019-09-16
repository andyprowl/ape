#include "Transformer.hpp"

#include "Transformable.hpp"

#include <algorithm>

Transformer::Transformer()
    : Transformer{glm::mat4{1.0f}}
{
}

Transformer::Transformer(glm::mat4 const & localTransformation)
    : Transformer{nullptr, localTransformation}
{
}

Transformer::Transformer(Transformer * const transformer, glm::mat4 const & localTransformation)
    : Transformable{transformer}
    , localTransformation{localTransformation}
    , globalTransformation{getContextTransformation() * localTransformation}
{
}

auto Transformer::getLocalTransformation() const
    -> glm::mat4
{
    return localTransformation;
}

auto Transformer::getGlobalTransformation() const
    -> glm::mat4
{
    return globalTransformation;
}

auto Transformer::setLocalTransformation(glm::mat4 const & newTransformation)
    -> void
{
    localTransformation = newTransformation;

    globalTransformation = getContextTransformation() * localTransformation;
}

// override (from Transformable)
auto Transformer::onContextTransformationChanged(glm::mat4 const & /*contextTransformation*/)
    -> void
{
    auto const transformation = getGlobalTransformation();

    for (auto const subject : subjects)
    {
        subject->onContextTransformationChanged(transformation);
    }
}

// To be invoked by Transformable.
auto Transformer::addSubject(Transformable & subject)
    -> void
{
    auto const it = findSubject(subject);

    assert(it != std::cend(subjects));

    subjects.push_back(&subject);
}

// To be invoked by Transformable.
auto Transformer::removeSubject(Transformable & subject)
    -> void
{
    auto const it = findSubject(subject);

    assert(it != std::cend(subjects));

    subjects.erase(it);
}

auto Transformer::findSubject(Transformable & subject)
    -> SubjectCollection::const_iterator
{
    return std::find(std::cbegin(subjects), std::cend(subjects), &subject);
}
