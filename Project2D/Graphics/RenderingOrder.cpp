#include "RenderingOrder.h"

#include <algorithm>
#include <cassert>

RenderingOrder::RenderingOrder(RenderingOrderType type) : type(type) {}

void RenderingOrder::release() {
    nodes = std::vector<RenderingOrderNode*>();
    spritesNodes = std::vector<RenderingOrderNode>();
    batchSpritesNode = std::vector<BatchRenderingOrderNode>();
    transforms = std::vector<TransformMatrix>();
}

void RenderingOrder::clear() {
    nodes.clear();
    spritesNodes.clear();
    batchSpritesNode.clear();
    transforms.clear();
}

RenderingOrderNode* RenderingOrder::operator[](size_t index) {
    return nodes[index];
}

const RenderingOrderNode* RenderingOrder::operator[](size_t index) const {
    return nodes[index];
}

void RenderingOrder::reserve(size_t size) {
    nodes.reserve(size);
    spritesNodes.reserve(size);
    batchSpritesNode.reserve(size);
    transforms.reserve(size);
}

void RenderingOrder::pushNode(ResourceReference materialResource, ResourceReference spriteResource, const TransformMatrix* transform) {
    RenderingOrderNode& node = spritesNodes.emplace_back();
    node.materialResource = materialResource;
    node.spriteResource = spriteResource;
    
    transforms.push_back(*transform);
    node.transform = &transforms.back();

    nodes.push_back(&node);
}

void RenderingOrder::pushBatchNode(ResourceReference materialResource, ResourceReference spriteResource, const TransformMatrix* transform,
    uint16_t vertecesCount, SpriteVertex* verteces, uint16_t indecesCount, uint16_t* indeces) {
    BatchRenderingOrderNode& node = batchSpritesNode.emplace_back();
    node.materialResource = materialResource;
    node.spriteResource = spriteResource;
    node.verteces = verteces;
    node.indeces = indeces;
    node.vertecesCount = vertecesCount;
    node.indecesCount = indecesCount;

    transforms.push_back(*transform);
    node.transform = &transforms.back();

    nodes.push_back(&node);
}

void RenderingOrder::sort() {
    std::stable_sort(nodes.begin(), nodes.end(), [](const RenderingOrderNode* node1, const RenderingOrderNode* node2) {
        TransformMatrix* transform1 = node1->transform;
        TransformMatrix* transform2 = node2->transform;


        if (transform1->_43 == transform2->_43) {
            if (node1->materialResource.getResourceID() == node2->materialResource.getResourceID()) {
                return node1->spriteResource.getResourceID() < node2->spriteResource.getResourceID();
            }
            else {
                return node1->materialResource.getResourceID() < node2->materialResource.getResourceID();
            }
        }
        else {
            return transform1->_43 < transform2->_43;
        }
        });
}
