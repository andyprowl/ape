#pragma once

#include <Glow/GpuResource/UniqueResource.hpp>

namespace glow
{

class GpuResource : public UniqueResource<unsigned int>
{

public:

    using UniqueResource::UniqueResource;

};

} // namespace glow
