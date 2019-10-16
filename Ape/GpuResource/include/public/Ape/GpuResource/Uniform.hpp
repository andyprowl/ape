#pragma once

#include <string>
#include <vector>

namespace ape
{

class ShaderProgram;

} // namespace ape

namespace ape::detail
{

template<typename T>
class BasicUniform
{

public:

    using ValueType = T;

public:

    BasicUniform(ShaderProgram & program, std::string const & name);

    BasicUniform(ShaderProgram & program, std::string const & name, T const & value);

    auto get() const
        -> T;

    auto set(T const & value)
        -> void;
    
protected:

    friend class ShaderProgram;

protected:

    BasicUniform(unsigned int const programId, int const location)
        : programId{programId}
        , location{location}
    {
    }

protected:

    unsigned int programId;
    
    int location;

};

} // namespace ape::detail

namespace ape
{

template<typename T>
class Uniform : public detail::BasicUniform<T>
{

public:

    using detail::BasicUniform<T>::BasicUniform;

    auto operator = (T const & value)
        -> Uniform &
    {
        this->set(value);

        return *this;
    }

};

template<>
class Uniform<bool> : public detail::BasicUniform<bool>
{

public:

    using detail::BasicUniform<bool>::BasicUniform;

    explicit operator bool () const
    {
        return this->get();
    }

    auto operator = (ValueType const value)
        -> Uniform &
    {
        set(value);

        return *this;
    }

};

} // namespace ape
