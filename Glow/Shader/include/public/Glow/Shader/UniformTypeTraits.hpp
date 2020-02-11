#pragma once

#include <glm/matrix.hpp>

#include <type_traits>

namespace glow
{

template<typename T>
class UniformBlockMember;

} // namespace glow

namespace glow::detail
{

template<typename T, typename = void>
class IsFundamental : public std::false_type
{
};

template<>
class IsFundamental<bool> : public std::true_type
{
};

template<>
class IsFundamental<int> : public std::true_type
{
};

template<>
class IsFundamental<float> : public std::true_type
{
};

template<>
class IsFundamental<double> : public std::true_type
{
};

template<glm::length_t L, typename T, glm::qualifier Q>
class IsFundamental<glm::vec<L, T, Q>>
{
};

template<glm::length_t N, glm::qualifier Q>
class IsFundamental<glm::mat<N, N, float, Q>> : public std::true_type
{
};

template<typename T, typename = void>
class IsMatrix : public std::false_type
{
};

template<glm::length_t N, glm::qualifier Q>
class IsMatrix<glm::mat<N, N, float, Q>> : public std::true_type
{
};

} // namespace glow::detail

namespace glow
{

template<typename T>
constexpr bool const isFundamental = detail::IsFundamental<T>{};

template<typename T>
constexpr bool const isMatrix = detail::IsMatrix<T>{};

} // namespace glow
