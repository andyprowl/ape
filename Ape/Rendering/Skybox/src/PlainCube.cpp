#include <Ape/Rendering/Skybox/PlainCube.hpp>

#include <Ape/Rendering/Skybox/PlainCubeVertex.hpp>

#include <Glow/BufferObject/VertexArrayObject.hpp>
#include <Glow/BufferObject/VertexLayout.hpp>
#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <array>

namespace ape
{

namespace
{

auto makeVertexBufferObject(std::vector<PlainCubeVertex> const & vertices)
    -> glow::VertexBufferObject
{
    auto vbo = glow::VertexBufferObject{};

    auto const binder = glow::bind(vbo);

    auto const vertexBufferSize = vertices.size() * sizeof(PlainCubeVertex);

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices.data(), GL_STATIC_DRAW);

    glow::sendVertexLayoutToGpu<PlainCubeVertex>();

    return vbo;
}

auto makeArrayObject(glow::VertexBufferObject const & vertexBuffer)
    -> glow::VertexArrayObject
{
    auto arrayObject = glow::VertexArrayObject{};

    auto const binder = glow::bind(arrayObject);

    vertexBuffer.bind();

    glow::sendVertexLayoutToGpu<PlainCubeVertex>();

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
    -> const glow::VertexBufferObject &
{
    return vertexBuffer;
}

auto PlainCube::getArrayObject() const 
    -> const glow::VertexArrayObject &
{
    return arrayObject;
}

auto PlainCube::getNumOfVertices() const
    -> int
{
    return 36;
}

auto PlainCube::makeVertices() const
    -> glow::VertexBufferObject
{
    auto vao = glow::VertexArrayObject{};

    auto const binder = glow::bind(vao);

    auto const vertices = PlainCubeVerticesBuilder{}.makeVertices();

    return makeVertexBufferObject(vertices);
}

} // namespace ape
