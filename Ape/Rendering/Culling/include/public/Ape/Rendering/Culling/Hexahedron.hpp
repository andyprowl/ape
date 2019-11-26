#pragma once

#include <array>

namespace ape
{

class Hexahedron
{

public:

    enum class Face
    {
        right,
        left,
        top,
        bottom,
        front,
        back
    };

    enum class Vertex
    {
        rightTopFront,
        rightTopBack,
        rightBottomFront,
        rightBottomBack,
        leftTopFront,
        leftTopBack,
        leftBottomFront,
        leftBottomBack
    };

};

inline auto getNeighborFaces(Hexahedron::Face const face)
    -> std::array<Hexahedron::Face, 4u> const &
{
    using Face = Hexahedron::Face;

    static auto const neighborhood = std::array<std::array<Face, 4u>, 6u>{{
        {{Face::back, Face::top, Face::front, Face::bottom}},
        {{Face::front, Face::bottom, Face::back, Face::top}},
        {{Face::front, Face::left, Face::back, Face::right}},
        {{Face::back, Face::right, Face::front, Face::left}},
        {{Face::left, Face::bottom, Face::right, Face::top}},
        {{Face::right, Face::bottom, Face::left, Face::top}}}};

    return neighborhood[static_cast<std::size_t>(face)];
}

inline auto getEdge(Hexahedron::Face const face1, Hexahedron::Face const face2)
    -> std::pair<Hexahedron::Vertex, Hexahedron::Vertex> const &
{
    using Vertex = Hexahedron::Vertex;

    static auto const invalid = std::make_pair(static_cast<Vertex>(-1), static_cast<Vertex>(-1));

    // Edge vertices are provided in clockwise ordering
    static auto const sharingTable = std::array<std::array<std::pair<Vertex, Vertex>, 6u>, 6u>{{
        // Face::right
        {{
            invalid, // Face::right
            invalid, // Face::left
            {Vertex::rightTopFront, Vertex::rightTopBack}, // Face::top
            {Vertex::rightBottomBack, Vertex::rightBottomFront}, // Face::bottom
            {Vertex::rightBottomFront, Vertex::rightTopFront}, // Face::front
            {Vertex::rightTopBack, Vertex::rightBottomBack}, // Face::back
        }},
        // Face::left
        {{
            invalid, // Face::right
            invalid, // Face::left
            {Vertex::leftTopBack, Vertex::leftTopFront}, // Face::top
            {Vertex::leftBottomFront, Vertex::leftBottomBack}, // Face::bottom
            {Vertex::leftTopFront, Vertex::leftBottomFront}, // Face::front
            {Vertex::leftBottomBack, Vertex::leftTopBack}, // Face::back
        }},
        // Face::top
        {{
            {Vertex::rightTopBack, Vertex::rightTopFront}, // Face::right
            {Vertex::leftTopFront, Vertex::leftTopBack}, // Face::left
            invalid, // Face::top
            invalid, // Face::bottom
            {Vertex::rightTopFront, Vertex::leftTopFront}, // Face::front
            {Vertex::leftTopBack, Vertex::rightTopBack}, // Face::back
        }},
        // Face::bottom
        {{
            {Vertex::rightBottomFront, Vertex::rightBottomBack}, // Face::right
            {Vertex::leftBottomBack, Vertex::leftBottomFront}, // Face::left
            invalid, // Face::top
            invalid, // Face::bottom
            {Vertex::leftBottomFront, Vertex::rightBottomFront}, // Face::front
            {Vertex::rightBottomBack, Vertex::leftBottomBack}, // Face::back
        }},
        // Face::front
        {{
            {Vertex::rightTopFront, Vertex::rightBottomFront}, // Face::right
            {Vertex::leftBottomFront, Vertex::leftTopFront}, // Face::left
            {Vertex::leftTopFront, Vertex::rightTopFront}, // Face::top
            {Vertex::rightBottomFront, Vertex::leftBottomFront}, // Face::bottom
            invalid, // Face::front
            invalid, // Face::back
        }},
        // Face::back
        {{
            {Vertex::rightBottomBack, Vertex::rightTopBack}, // Face::right
            {Vertex::leftTopBack, Vertex::leftBottomBack}, // Face::left
            {Vertex::rightTopBack, Vertex::leftTopBack}, // Face::top
            {Vertex::leftBottomBack, Vertex::rightBottomBack}, // Face::bottom
            invalid, // Face::front
            invalid, // Face::back
        }}}};

    return sharingTable[static_cast<std::size_t>(face1)][static_cast<std::size_t>(face2)];
}

inline auto getAllFaces()
    -> std::array<Hexahedron::Face, 6u>
{
    using Face = Hexahedron::Face;

    static auto const faces = std::array<Hexahedron::Face, 6u>{{
        Face::right,
        Face::left,
        Face::top,
        Face::bottom,
        Face::front,
        Face::back}};

    return faces;
}

} // namespace ape
