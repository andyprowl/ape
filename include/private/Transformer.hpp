#pragma once

#include "Transformable.hpp"

#include <glm/mat4x4.hpp>

#include <vector>

class Transformer : public Transformable
{

public:

    Transformer();

    explicit Transformer(glm::mat4 const & localTransformation);

    Transformer(Transformer * transformer, glm::mat4 const & localTransformation);

    auto getLocalTransformation() const
        -> glm::mat4;

    auto getGlobalTransformation() const
        -> glm::mat4;

    auto setLocalTransformation(glm::mat4 const & newTransformation)
        -> void;

private:

    friend class Transformable;

    using SubjectCollection = std::vector<Transformable *>;

private:

    // virtual (from Transformable)
    auto onContextTransformationChanged(glm::mat4 const & contextTransformation)
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

    glm::mat4 localTransformation;

    glm::mat4 globalTransformation;

};