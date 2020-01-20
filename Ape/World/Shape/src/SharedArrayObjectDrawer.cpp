#include <Ape/World/Shape/SharedArrayObjectDrawer.hpp>

#include <Ape/World/Shape/Shape.hpp>

#include <Glow/BufferObject/VertexLayout.hpp>
#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

namespace ape
{

SharedArrayObjectDrawer::SharedArrayObjectDrawer()
{
    auto const layout = glow::getVertexLayout<ShapeVertex>();

    auto const & attributes = layout.getAttributes();

    for (auto i = 0; i < static_cast<int>(attributes.size()); ++i)
    {
        auto binding = arrayObject.getBinding(i);

        binding.setAttributeIndex(i);

        binding.setFormat(attributes[i]);

        arrayObject.enableAttribute(i);
    }
}

// virtual (from ShapeDrawer)
auto SharedArrayObjectDrawer::beginRenderBatch()
    -> void
{
    arrayObject.bind();
}

auto SharedArrayObjectDrawer::render(Shape const & shape)
    -> void
{
    for (auto i = 0; i < ShapeVertex::numOfComponents; ++i)
    {
        auto binding = arrayObject.getBinding(i);
        
        binding.setVertexSource(shape.getVertexBufferObject(), 0, sizeof(ShapeVertex));
    }

    arrayObject.setIndexSource(shape.getElementBufferObject());

    auto const numOfVertices = shape.getNumOfVertices();

    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);
}

// virtual (from ShapeDrawer)
auto SharedArrayObjectDrawer::endRenderBatch()
    -> void
{
    arrayObject.unbind();
}

} // namespace ape
