#pragma once

#include <glm/vec3.hpp>

#include <string>
#include <vector>

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

template<typename T>
class Uniform<std::vector<T>>
{

public:

    using ValueType = std::vector<typename T::ValueType>;

public:

    Uniform(ShaderProgram const & program, std::string const & prefix)
        : program{&program}
        , prefix{prefix}
        , arraySize{program, prefix + "ArraySize"}
    {
    }

    Uniform(ShaderProgram const & program, std::string prefix, int const size)
        : Uniform{program, std::move(prefix)}
    {
        resize(size);
    }

    auto operator [] (int const i)
        -> T &
    {
        if (i >= static_cast<int>(uniforms.size()))
        {
            resize(i + 1);
        }

        return uniforms[i];
    }

    auto resize(int const newSize)
        -> void
    {
        if (newSize == static_cast<int>(uniforms.size()))
        {
            return;
        }

        arraySize.set(newSize);

        if (newSize > static_cast<int>(uniforms.size()))
        {
            for (auto i = static_cast<int>(uniforms.size()); i < newSize; ++i)
            {
                uniforms.emplace_back(*program, prefix + "[" + std::to_string(i) + "]");
            }
        }
        else
        {
            uniforms.erase(std::cbegin(uniforms) + newSize, std::cend(uniforms));
        }
    }

    auto get() const
        -> ValueType
    {
        auto values = ValueType{};

        for (auto const & uniform : uniforms)
        {
            auto value = uniform.get();

            values.push_back(std::move(value));
        }

        return values;
    }

    auto set(ValueType const & values)
        -> void
    {
        auto numOfValues = static_cast<int>(values.size());

        resize(numOfValues);

        for (auto i = 0; i < numOfValues; ++i)
        {
            uniforms[i] = values[i];
        }
    }

    auto operator = (ValueType const & values)
        -> Uniform &
    {
        set(values);

        return *this;
    }

private:

    ShaderProgram const * program;

    std::string prefix;

    Uniform<int> arraySize;

    std::vector<T> uniforms;

};
