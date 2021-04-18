#include "D3D11BufferRenderingOrder.h"

void D3D11BufferRenderingOrder::pushNode(EntityID entityID, ResourceReference materialResource, ResourceReference spriteResource,
	ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, size_t indexCount, const TransformMatrix* transform) {
    D3D11BufferRenderingOrderNode* node = new (nodes + currentNodeSize) D3D11BufferRenderingOrderNode();
    node->entityID = entityID;
    node->materialResource = materialResource;
    node->spriteResource = spriteResource;
    node->vertexBuffer = vertexBuffer;
    node->indexBuffer = indexBuffer;
    node->indexCount = indexCount;
    node->transform = transforms + currentNodeSize;

    memcpy_s(transforms + currentNodeSize, sizeof(TransformMatrix), transform, sizeof(TransformMatrix));
    ++currentNodeSize;
}

void D3D11BufferRenderingOrder::sort() {
    std::qsort(nodes, currentNodeSize, sizeof(D3D11BufferRenderingOrderNode), [](const void* node1, const void* node2) {
        ResourceID materialResID1 = ((D3D11BufferRenderingOrderNode*)(node1))->materialResource.getResourceID();
        ResourceID materialResID2 = ((D3D11BufferRenderingOrderNode*)(node2))->materialResource.getResourceID();
        ResourceID spriteResID1 = ((D3D11BufferRenderingOrderNode*)(node1))->spriteResource.getResourceID();
        ResourceID spriteResID2 = ((D3D11BufferRenderingOrderNode*)(node2))->spriteResource.getResourceID();

        if ((materialResID1 > materialResID2) || ((materialResID1 == materialResID2) && (spriteResID1 > spriteResID2))) {
            return 1;
        }
        if ((materialResID1 < materialResID2) || ((materialResID1 == materialResID2) && (spriteResID1 < spriteResID2))) {
            return -1;
        }

        return 0;
        });
}
