#pragma once

#include "TextureDescriptor.hpp"

class Texture
{

public:

    explicit Texture(std::string filename);
    
    Texture(TextureDescriptor descriptor, std::string filename);

    auto bind(int unit) const
        -> void;

    auto getFilename() const
        -> std::string;

    auto getFormat() const
        -> GLenum;

    auto getSize() const
        -> Size<int>;

private:

    int id;

    std::string filename;

    GLenum format;

    Size<int> size;

};
