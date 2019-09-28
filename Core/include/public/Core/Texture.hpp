#pragma once

#include <Core/TextureDescriptor.hpp>

namespace ape
{

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

} // namespace ape
