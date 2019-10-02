#pragma once

namespace ape
{

class BufferObject
{

public:

    BufferObject(BufferObject const & rhs) = delete;

    BufferObject(BufferObject && rhs) noexcept;

    auto operator = (BufferObject const & rhs)
        -> BufferObject & = delete;

    auto operator = (BufferObject && rhs) noexcept
        -> BufferObject &;

    ~BufferObject();

protected:
    
    explicit BufferObject(unsigned int id);

protected:

    unsigned int id;

private:

    auto destroy()
        -> void;

};

} // namespace ape
