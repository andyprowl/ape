#pragma once

#include <string>

class Material;
class Shape;

class Mesh
{

public:

    Mesh(std::string name, Shape const & shape, Material const & material);

    auto getName() const
        -> std::string;

    auto getShape() const
        -> Shape const &;

    auto getMaterial() const
        -> Material const &;

private:

    std::string name;

    Shape const * shape;

    Material const * material;

};
