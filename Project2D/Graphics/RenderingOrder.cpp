#include "RenderingOrder.h"

#include <algorithm>
#include <cassert>

RenderingOrder::RenderingOrder(RenderingOrderType type)
    : nodes(nullptr),
    transforms(nullptr),
    heap(nullptr),
    allocatedNodesSize(0),
    currentNodeSize(0),
    type(type) {}

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
    std::sort(nodes, nodes + currentNodeSize, [](const RenderingOrderNode& node1, const RenderingOrderNode& node2) {
        TransformMatrix* transform1 = node1.transform;
        TransformMatrix* transform2 = node2.transform;

        
        if (transform1->_43 == transform2->_43) {
            if (node1.materialResource.getResourceID() == node2.materialResource.getResourceID()) {
                return node1.spriteResource.getResourceID() < node2.spriteResource.getResourceID();
            }
            else {
                return node1.materialResource.getResourceID() < node2.materialResource.getResourceID();
            }
        }
        else {
            return transform1->_43 < transform2->_43;
        }
        });
}
