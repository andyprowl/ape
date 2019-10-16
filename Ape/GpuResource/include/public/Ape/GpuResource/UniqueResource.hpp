#pragma once

#include <functional>
#include <memory>

namespace ape
{

template<typename IdType>
class UniqueResourceDeleter : public std::function<auto (IdType) -> void>
{

public:

    using pointer = IdType;

    using Base = std::function<auto (IdType) -> void>;

public:

    using Base::Base;

};

template<typename IdType>
class UniqueResource : public std::unique_ptr<IdType, UniqueResourceDeleter<IdType>>
{

public:

    using Base = std::unique_ptr<IdType, UniqueResourceDeleter<IdType>>;

    using Id = IdType;

public:

    using Base::Base;

};

} // namespace ape
