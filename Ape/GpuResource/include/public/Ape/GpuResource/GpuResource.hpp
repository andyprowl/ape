#pragma once

#include <Ape/GpuResource/UniqueResource.hpp>

namespace ape
{

class GpuResource : public UniqueResource<unsigned int>
{

public:

    using UniqueResource::UniqueResource;

};

} // namespace ape
