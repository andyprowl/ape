#pragma once

#include "Uniform.hpp"

#include <string>
#include <vector>

template<typename T>
class UniformArray
{

public:

    using ValueType = std::vector<typename T::ValueType>;

public:

    UniformArray(ShaderProgram const & program, std::string const & prefix)
        : program{&program}
        , prefix{prefix}
        , arraySize{program, prefix + "ArraySize"}
    {
    }

    UniformArray(ShaderProgram const & program, std::string prefix, int const size)
        : UniformArray{program, std::move(prefix)}
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
        -> UniformArray<T> &
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
