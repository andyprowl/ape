#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <array>
#include <cstddef>

using GLenum = unsigned int;

namespace glow
{

enum class DataType
{
    byte,
    unsignedByte,
    shortInteger,
    unsignedShortInteger,
    integer,
    unsignedInteger,
    floatingPoint,
    doubleFloatingPoint,
    unknown
};

auto convertToOpenGLDataType(DataType type)
    -> GLenum;

auto getDataTypeSize(DataType type)
    -> int;

template<typename T>
auto asDataType()
    -> DataType
{
    if constexpr (std::is_same_v<T, std::byte> || std::is_same_v<T, char>)
    {
        return DataType::byte;
    }
    else if constexpr (std::is_same_v<T, unsigned char>)
    {
        return DataType::unsignedByte;
    }
    else if constexpr (std::is_same_v<T, short>)
    {
        return DataType::shortInteger;
    }
    else if constexpr (std::is_same_v<T, unsigned short>)
    {
        return DataType::unsignedShortInteger;
    }
    else if constexpr (std::is_same_v<T, int>)
    {
        return DataType::integer;
    }
    else if constexpr (std::is_same_v<T, unsigned int>)
    {
        return DataType::unsignedInteger;
    }
    else if constexpr (std::is_same_v<T, float>)
    {
        return DataType::floatingPoint;
    }
    else if constexpr (std::is_same_v<T, double>)
    {
        return DataType::doubleFloatingPoint;
    }
    else
    {
        static_assert(false, "Data type not supported!");

        return DataType::unknown;
    }
}

} // namespace glow

namespace glow::detail
{

template<typename T>
class BaseType
{

public:

    using type = T;

};

template<glm::length_t L, typename T, glm::qualifier Q>
class BaseType<glm::vec<L, T, Q>>
{

public:

    using type = T;

};

} // namespace glow::detail

namespace glow
{

template<typename T>
using BaseType = typename detail::BaseType<T>::type;

} // namespace glow
