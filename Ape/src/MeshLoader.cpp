#include <Ape/MeshLoader.hpp>

#include <assimp/scene.h>

namespace ape
{

namespace
{

auto makeVector(aiVector3D const & v)
    -> glm::vec3
{
    return {v.x, v.y, v.z};
}

} // unnamed namespace

MeshLoader::MeshLoader(AssetRepository & assets)
    : assets{&assets}
{
}

auto MeshLoader::load(aiScene const & scene) const
    -> void
{
    assets->shapes.reserve(scene.mNumMeshes);

    for (auto i = 0u; i < scene.mNumMeshes; ++i)
    {
        auto const mesh = scene.mMeshes[i];

        importMesh(*mesh);
    }
}

auto MeshLoader::importMesh(aiMesh const & mesh) const
    -> void
{
    auto const & shape = importShape(mesh);

    auto const & material = assets->materials[mesh.mMaterialIndex];

    auto name = std::string{mesh.mName.C_Str()};

    assets->meshes.emplace_back(std::move(name), shape, material);
}

auto MeshLoader::importShape(aiMesh const & mesh) const
    -> Shape const &
{
    auto vertices = importVertices(mesh);

    auto indices = importIndices(mesh);

    return assets->shapes.emplace_back(std::move(vertices), std::move(indices));
}

auto MeshLoader::importVertices(aiMesh const & mesh) const
    -> std::vector<Vertex>
{
    auto vertices = std::vector<Vertex>{};

    for (auto i = 0u; i < mesh.mNumVertices; ++i)
    {
        auto const position = makeVector(mesh.mVertices[i]);

        auto const normal = makeVector(mesh.mNormals[i]);

        auto const textureCoords = glm::vec2{makeVector(mesh.mTextureCoords[0][i])};

        vertices.emplace_back(position, normal, textureCoords);
    }

    return vertices;
}

auto MeshLoader::importIndices(aiMesh const & mesh) const
    -> std::vector<unsigned int>
{
    auto indices = std::vector<unsigned int>{};

    for (auto i = 0u; i < mesh.mNumFaces; ++i)
    {
        for (auto j = 0u; j < mesh.mFaces[i].mNumIndices; ++j)
        {
            indices.push_back(mesh.mFaces[i].mIndices[j]);
        }
    }

    return indices;
}

} // namespace ape
