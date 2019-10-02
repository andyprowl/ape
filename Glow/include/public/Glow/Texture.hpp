#pragma once

#include <Glow/TextureDescriptor.hpp>

namespace ape
{

class Texture
{

public:
    
    explicit Texture(TextureDescriptor descriptor);

    Texture(Texture const & rhs) = delete;

    Texture(Texture && rhs) noexcept;

    auto operator = (Texture const & rhs)
        -> Texture & = delete;

    auto operator = (Texture && rhs) noexcept
        -> Texture &;

    ~Texture();

    auto bind(int unit) const
        -> void;

    auto getFormat() const
        -> unsigned int;

    auto getSize() const
        -> Size<int>;

private:

    auto destroy()
        -> void;

private:

    unsigned int id;

    unsigned int format;

    Size<int> size;

};

} // namespace ape
