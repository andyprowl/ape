#pragma once

#include <Ape/Vertex.hpp>

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

    Shape(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices);

    Shape(Shape const & rhs) = delete;

    Shape(Shape && rhs) noexcept;

    auto operator = (Shape const & rhs)
        -> Shape & = delete;

    auto operator = (Shape && rhs) noexcept
        -> Shape &;

    ~Shape();

    auto draw() const
        -> void;

private:

    ObjectIdSet objectIds;

    int numOfVertices;

};
