#pragma once

#include <Ape/TextureDescriptor.hpp>

class Texture
{

public:

    explicit Texture(std::string filepath);
    
    Texture(TextureDescriptor descriptor, std::string filepath);

    auto bind(int unit) const
        -> void;

    auto getFilepath() const
        -> std::string;

    auto getFormat() const
        -> unsigned int;

    auto getSize() const
        -> Size<int>;

private:

    int id;

    std::string filepath;

    unsigned int format;

    Size<int> size;

};