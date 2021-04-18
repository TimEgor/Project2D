#include "SpriteRenderingOrder.h"

#include <cassert>

void SpriteRenderingOrder::pushNode(EntityID entityID, ResourceReference materialResource, ResourceReference spriteResource, const TransformMatrix* transform) {
    SpriteRenderingOrderNode* node = new (nodes + currentNodeSize) SpriteRenderingOrderNode();
    node->entityID = entityID;
    node->materialResource = materialResource;
    node->spriteResource = spriteResource;
    node->transform = transforms + currentNodeSize;

    memcpy_s(transforms + currentNodeSize, sizeof(TransformMatrix), transform, sizeof(TransformMatrix));
    ++currentNodeSize;
}

void SpriteRenderingOrder::sort() {
    std::qsort(nodes, currentNodeSize, sizeof(SpriteRenderingOrderNode), [](const void* node1, const void* node2) {
        ResourceID materialResID1 = ((SpriteRenderingOrderNode*)(node1))->materialResource.getResourceID();
        ResourceID materialResID2 = ((SpriteRenderingOrderNode*)(node2))->materialResource.getResourceID();
        ResourceID spriteResID1 = ((SpriteRenderingOrderNode*)(node1))->spriteResource.getResourceID();
        ResourceID spriteResID2 = ((SpriteRenderingOrderNode*)(node2))->spriteResource.getResourceID();

        if ((materialResID1 > materialResID2) || ((materialResID1 == materialResID2) && (spriteResID1 > spriteResID2))) {
            return 1;
        }
        if ((materialResID1 < materialResID2) || ((materialResID1 == materialResID2) && (spriteResID1 < spriteResID2))) {
            return -1;
        }

        return 0;
        });
}
