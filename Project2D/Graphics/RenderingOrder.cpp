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

    currentNodeSize = 0;
    allocatedNodesSize = 0;
}

void RenderingOrder::clear() {
    currentNodeSize = 0;
}

const RenderingOrderNode& RenderingOrder::operator[](size_t index) {
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
}

void RenderingOrder::pushNode(const RenderingOrderNode& node) {
    nodes[currentNodeSize] = node;
    ++currentNodeSize;
}

void RenderingOrder::pushNode(RenderingOrderNode&& node) {
    nodes[currentNodeSize] = std::move(node);
    ++currentNodeSize;
}
