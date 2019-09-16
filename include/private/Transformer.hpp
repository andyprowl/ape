#pragma once

#include "Transformable.hpp"

#include <glm/mat4x4.hpp>

#include <vector>

class Transformer : public Transformable
{

public:

    using Transformable::Transformable;

private:

    friend class Transformable;

    using SubjectCollection = std::vector<Transformable *>;

private:

    // virtual (from Transformable)
    auto onTransformationChanged(BaseTransformationKind kind)
        -> void override;

    // To be invoked by Transformable.
    auto addSubject(Transformable & subject)
        -> void;

    // To be invoked by Transformable.
    auto removeSubject(Transformable & subject)
        -> void;

    auto findSubject(Transformable & subject)
        -> SubjectCollection::const_iterator;

private:

    SubjectCollection subjects;

};