#pragma once

#include <glm/vec3.hpp>

#include <vector>

class Shape
{

public:

    class ObjectIdSet
    {

    public:

        ObjectIdSet(
            unsigned int const vertexBufferObjectId,
            unsigned int const elementBufferObjectId,
            unsigned int const vertexArrayObjectId)
            : vertexBufferObjectId{vertexBufferObjectId}
            , elementBufferObjectId{elementBufferObjectId}
            , vertexArrayObjectId{vertexArrayObjectId}
        {
        }

    public:

        unsigned int vertexBufferObjectId;

        unsigned int elementBufferObjectId;

        unsigned int vertexArrayObjectId;

    };

public:

    Shape(std::vector<float> const & vertices, std::vector<unsigned int> const & indices);

    Shape(Shape && rhs) noexcept;

    auto operator = (Shape && rhs) noexcept
        -> Shape &;

    ~Shape();

    auto draw() const
        -> void;

private:

    ObjectIdSet objectIds;

    int numOfVertices;

};
