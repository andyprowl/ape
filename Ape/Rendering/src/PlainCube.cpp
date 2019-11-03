#include <Ape/Rendering/PlainCube.hpp>

#include <Ape/Rendering/PlainCubeVertex.hpp>

#include <Ape/GpuResource/ScopedBinder.hpp>
#include <Ape/GpuResource/VertexArrayObject.hpp>
#include <Ape/GpuResource/VertexLayout.hpp>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <array>

namespace ape
{

namespace
{

auto makeVertexBufferObject(std::vector<PlainCubeVertex> const & vertices)
    -> VertexBufferObject
{
    auto vbo = VertexBufferObject{};

    auto const binder = bind(vbo);

    auto const vertexBufferSize = vertices.size() * sizeof(PlainCubeVertex);

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices.data(), GL_STATIC_DRAW);

    sendVertexLayoutToGpu<PlainCubeVertex>();

    return vbo;
}

auto makeArrayObject(VertexBufferObject const & vertexBuffer)
    -> VertexArrayObject
{
    auto arrayObject = VertexArrayObject{};

    auto const binder = bind(arrayObject);

    vertexBuffer.bind();

    sendVertexLayoutToGpu<PlainCubeVertex>();

    return arrayObject;
}

auto rotate(float const degrees, glm::vec3 const & axis)
    -> glm::mat4
{
    return glm::rotate(glm::mat4{1.0f}, glm::radians(degrees), axis);
}

auto scale(glm::vec3 const & factor)
    -> glm::mat4
{
    return glm::scale(glm::mat4{1.0f}, factor);
}

auto translate(glm::vec3 const & offset)
    -> glm::mat4
{
    return glm::translate(glm::mat4{1.0f}, offset);
}

auto transform(glm::vec3 const & v, glm::mat4 const & transformation)
    -> glm::vec3
{
    return glm::vec3{transformation * glm::vec4{v, 1.0f}};
}

class PlainCubeVerticesBuilder
{

public:

    auto makeVertices()
        -> std::vector<PlainCubeVertex>
    {
        auto const face = getBottomFace();

        // bottom
        addScaledFace(face, translate({0.0f, -0.5f, 0.0f}));
        
        // top
        addScaledFace(face, translate({0.0f, 0.5f, 0.0f}) * rotate(180.0f, {1.0f, 0.0f, 0.0f}));

        // front
        addScaledFace(face, translate({0.0f, 0.0f, 0.5f}) * rotate(-90.0f, {1.0f, 0.0f, 0.0f}));

        // back
        addScaledFace(face, translate({0.0f, 0.0f, -0.5f}) * rotate(90.0f, {1.0f, 0.0f, 0.0f}));

        // left
        addScaledFace(face, translate({-0.5f, 0.0f, 0.0f}) * rotate(-90.0f, {0.0f, 0.0f, 1.0f}));

        // right
        addScaledFace(face, translate({0.5f, 0.0f, 0.0f}) * rotate(90.0f, {0.0f, 0.0f, 1.0f}));

        return std::move(vertices);
    }

private:

    using Face = std::array<PlainCubeVertex, 6u>;

private:

    auto getBottomFace() const
        -> Face
    {
        // NOTICE: clockwise winding order is important, because we will always be watching this
        // skybox geometry "from inside".

        return {{
            PlainCubeVertex{{-0.5f, 0.0f, -0.5f}},
            PlainCubeVertex{{+0.5f, 0.0f, +0.5f}},
            PlainCubeVertex{{+0.5f, 0.0f, -0.5f}},
            PlainCubeVertex{{-0.5f, 0.0f, +0.5f}},
            PlainCubeVertex{{+0.5f, 0.0f, +0.5f}},
            PlainCubeVertex{{-0.5f, 0.0f, -0.5f}}}};
    }

    auto addScaledFace(Face const & face, glm::mat4 const & transformation)
        -> void
    {
        for (auto const & v : face)
        {
            auto const position = transform(v.position, transformation);

            vertices.emplace_back(position);
        }
    }

private:

    std::vector<PlainCubeVertex> vertices;

};

} // unnamed namespace

PlainCube::PlainCube()
    : vertexBuffer{makeVertices()}
    , arrayObject{makeArrayObject(vertexBuffer)}
{
}

auto PlainCube::getVertexBuffer() const 
    -> const VertexBufferObject &
{
    return vertexBuffer;
}

auto PlainCube::getArrayObject() const 
    -> const VertexArrayObject &
{
    return arrayObject;
}

auto PlainCube::getNumOfVertices() const
    -> int
{
    return 36;
}

auto PlainCube::makeVertices() const
    -> VertexBufferObject
{
    auto vao = VertexArrayObject{};

    auto const binder = bind(vao);

    auto const vertices = PlainCubeVerticesBuilder{}.makeVertices();

    return makeVertexBufferObject(vertices);
}

} // namespace ape
