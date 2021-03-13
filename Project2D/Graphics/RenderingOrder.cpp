#include "RenderingOrder.h"

#include <cassert>

bool RenderingOrder::init(Heap* _heap) {
    heap = _heap;

    currentNodeSize = 0;
    allocatedNodesSize = 0;

    return true;
}

void RenderingOrder::release() {
    if (nodes) {
        heap->deallocate(nodes);
        nodes = nullptr;
    }

    if (transforms) {
        heap->deallocate(transforms);
        transforms = nullptr;
    }

    currentNodeSize = 0;
    allocatedNodesSize = 0;
}

void RenderingOrder::clear() {
    currentNodeSize = 0;
}

RenderingOrderNode& RenderingOrder::operator[](size_t index) {
    assert(nodes && "RenderingOrder buffer hasn't been allocated.");
    assert(index < currentNodeSize);

    return nodes[index];
}

const RenderingOrderNode& RenderingOrder::operator[](size_t index) const {
    assert(nodes && "RenderingOrder buffer hasn't been allocated.");
    assert(index < currentNodeSize);

    return nodes[index];
}

void RenderingOrder::preReSize(size_t size) {
    if (!nodes) {
        nodes = (RenderingOrderNode*)(heap->allocate(size * sizeof(RenderingOrderNode)));
    }
    else {
        if (allocatedNodesSize < size) {
            heap->reallocate(nodes, size * sizeof(RenderingOrderNode));
        }
    }

    if (!transforms) {
        transforms = (TransformMatrix*)(heap->allocate(size * sizeof(TransformMatrix)));
    }
    else {
        if (allocatedNodesSize < size) {
            heap->reallocate(transforms, size * sizeof(TransformMatrix));
        }
    }

    if (allocatedNodesSize < size) {
        allocatedNodesSize = size;
    }
}

void RenderingOrder::pushNode(EntityID entityID, ResourceReference materialResource, ResourceReference spriteResource, const TransformMatrix* transform) {
    RenderingOrderNode* node = new (nodes + currentNodeSize) RenderingOrderNode();
    node->entityID = entityID;
    node->materialResource = materialResource;
    node->spriteResource = spriteResource;
    node->transform = transforms + currentNodeSize;

    memcpy_s(transforms + currentNodeSize, sizeof(TransformMatrix), transform, sizeof(TransformMatrix));
    ++currentNodeSize;
}

void RenderingOrder::sort() {
    std::qsort(nodes, currentNodeSize, sizeof(RenderingOrderNode), [](const void* node1, const void* node2) {
        ResourceID materialResID1 = ((RenderingOrderNode*)(node1))->materialResource.getResourceID();
        ResourceID materialResID2 = ((RenderingOrderNode*)(node2))->materialResource.getResourceID();
        ResourceID spriteResID1 = ((RenderingOrderNode*)(node1))->spriteResource.getResourceID();
        ResourceID spriteResID2 = ((RenderingOrderNode*)(node2))->spriteResource.getResourceID();

        if ((materialResID1 > materialResID2) || ((materialResID1 == materialResID2) && (spriteResID1 > spriteResID2))) {
            return 1;
        }
        if ((materialResID1 < materialResID2) || ((materialResID1 == materialResID2) && (spriteResID1 < spriteResID2))) {
            return -1;
        }

        return 0;
        });
}
