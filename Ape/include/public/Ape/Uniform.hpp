#pragma once

#include <glm/vec3.hpp>

#include <string>
#include <vector>

class ShaderProgram;

namespace detail
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

    BasicUniform(int const programId, int const location)
        : programId{programId}
        , location{location}
    {
    }

protected:

    int programId;
    
    int location;

};

} // namespace detail

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

template<typename T>
class Uniform<std::vector<T>>
{

public:

    using ValueType = std::vector<typename T::ValueType>;

public:

    Uniform(ShaderProgram & program, std::string const & prefix)
        : program{&program}
        , prefix{prefix}
        , arraySize{program, prefix + "ArraySize"}
    {
    }

    Uniform(ShaderProgram & program, std::string prefix, int const size)
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

    ShaderProgram * program;

    std::string prefix;

    Uniform<int> arraySize;

    std::vector<T> uniforms;

};
