#include "Transformer.hpp"

#include "Transformable.hpp"

#include <algorithm>

// override (from Transformable)
auto Transformer::onTransformationChanged(BaseTransformationKind const)
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
