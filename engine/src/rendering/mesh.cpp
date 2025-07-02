#include "rendering/mesh.h"
#include "rendering/renderer.h"
#include "utility/managed_memory_block.h"


using namespace eloo;

namespace {
    struct mesh_metadata {
        mesh_metadata() = default;
        vertex_buffer::id_t     vertexBuffer = vertex_buffer::id_t::INVALID;
        index_buffer::id_t      indexBuffer = index_buffer::id_t::INVALID;
        uint32_t                submeshStart = 0;
        uint32_t                submeshCount = 0;
    };

    struct submesh_metadata {
        uint32_t                indexOffset;
        uint32_t                indexCount;
#if defined(MATERIALS_IMPLEMENTED)
        material::id_t          materialID;
#endif
    };


    constexpr int MEMORY_BLOCK_INITIAL_SIZE = 1000;
    constexpr float MEMORY_BLOCK_EXPANSION = 0.4f;

    static managed_memory_block<mesh_metadata, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION>       gMeshMetadataBlock;
    static managed_memory_block<submesh_metadata, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION>    gSubmeshBlock;
    static managed_memory_block<uint32_t, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION>            gIndexBlock;
}

mesh::id_t mesh::create(const mesh::data& meshData, renderer_interface* renderer) ELOO_NOEXCEPT {

    // Store vertices
    const size_t vertCount = meshData.vertices.size();
    const size_t vertBufSize = vertCount * sizeof(mesh::vertex);
    uint8_t* rawVertexData = static_cast<uint8_t*>(::operator new(vertBufSize));
    memcpy(rawVertexData, meshData.vertices.data(), vertBufSize);

    // Store indices
    size_t totalIndices = 0;
    const size_t submeshIndexDataStart = gSubmeshBlock.count();
    for (auto& submesh : meshData.sections) {
        const size_t indexCount = submesh.indices.size();
        totalIndices += indexCount;

        const size_t id = gSubmeshBlock.emplace();
        gSubmeshBlock[id].indexOffset = gIndexBlock.push(submesh.indices.data(), indexCount);
        gSubmeshBlock[id].indexCount = static_cast<uint32_t>(indexCount);
#if defined(MATERIALS_IMPLEMENTED)
        gSubmeshBlock[id].materialID = submesh.materialID;
#endif
    }
    const size_t indexBufSize = totalIndices * sizeof(uint32_t);
    uint8_t* rawIndexData = static_cast<uint8_t*>(::operator new(indexBufSize));
    memcpy(rawIndexData, &gIndexBlock.get(submeshIndexDataStart), indexBufSize);

    // Populate mesh_metadata
    const id_t meshID = gMeshMetadataBlock.emplace();
    gMeshMetadataBlock[meshID].vertexBuffer = renderer->create_vertex_buffer(rawVertexData, vertBufSize, false);
    gMeshMetadataBlock[meshID].indexBuffer = renderer->create_index_buffer(rawIndexData, indexBufSize, false);
    gMeshMetadataBlock[meshID].submeshStart = static_cast<uint32_t>(submeshIndexDataStart);
    gMeshMetadataBlock[meshID].submeshCount = static_cast<uint32_t>(meshData.sections.size());

    renderer->register_mesh(meshID);

    ::operator delete(rawVertexData);
    ::operator delete(rawIndexData);

    return meshID;
}

bool mesh::release(mesh::id_t meshID, renderer_interface* renderer) ELOO_NOEXCEPT {
    if (!is_valid(meshID)) {
        return false; // Invalid mesh ID
    }

    renderer->deregister_mesh(meshID);
    renderer->destroy_index_buffer(gMeshMetadataBlock[meshID].indexBuffer);
    renderer->destroy_vertex_buffer(gMeshMetadataBlock[meshID].vertexBuffer);
    gMeshMetadataBlock.remove(meshID);

    return true;
}

bool mesh::is_valid(mesh::id_t meshID, int submesh) ELOO_NOEXCEPT {
    return gMeshMetadataBlock.is_valid(meshID) &&
        (submesh <= 0 || submesh < gMeshMetadataBlock[meshID].submeshCount);
}

#if defined(MATERIALS_IMPLEMENTED)
bool mesh::try_set_material(mesh::id_t meshID, int submeshIndex, material::id_t materialID) ELOO_NOEXCEPT {
    if (!is_valid(meshID) || !material::is_valid(materialID)) {
        return false;
    }
    const auto& meta = gMeshMetadataBlock.get(meshID);
    if (submeshIndex < 0 || submeshIndex >= meta.submeshCount) {
        return false; // Invalid submesh index
    }
    auto& submesh = gSubmeshBlock.get(meta.submeshStart + submeshIndex);
    submesh.materialID = materialID;
    return true;
}
#endif // MATERIALS_IMPLEMENTED