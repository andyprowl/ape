#pragma once

#include <glm/vec3.hpp>

#include <string>

class ShaderProgram;

template<typename T>
class Uniform
{

public:

    using ValueType = T;

public:

    Uniform(ShaderProgram const & program, std::string const & name);

    auto get() const
        -> T;

    auto set(T const & value)
        -> void;

    auto operator = (T const & value)
        -> Uniform &
    {
        set(value);

        return *this;
    }

private:

    friend class ShaderProgram;

private:

    Uniform(int const programId, int const location)
        : programId{programId}
        , location{location}
    {
    }

private:

    int programId;
    
    int location;

};
