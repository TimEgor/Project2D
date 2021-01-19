#include "Scene.h"

#include <MemoryManager/MemoryManager.h>

bool Scene::init() {
    nodes.reserve(NODES_ALLOCATOR_SIZE);
    return nodeAllocators.init(MemoryManager::get().getDefaultHeap(), sizeof(Node), NODES_ALLOCATOR_SIZE);
}

void Scene::release() {
    nodes = std::unordered_map<NodeID, NodeHandler>();
    nodeAllocators.release();
}

Node* Scene::createNode(NodeID id) {
    size_t oldAllocatorCount = nodeAllocators.size();

    PoolAllocatorVector::AllocationInfo allocationInfo = nodeAllocators.allocate();
    Node* newNode = new (allocationInfo.allocationAddress) Node();

    size_t newAllocatorCount = nodeAllocators.size();
    if (oldAllocatorCount < newAllocatorCount) {
        nodes.reserve(newAllocatorCount * NODES_ALLOCATOR_SIZE);
    }

    nodes.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, *newNode, allocationInfo.allocatorIndex));

    return newNode;
}

void Scene::deleteNode(NodeID id) {
    auto findIter = nodes.find(id);
    if (findIter != nodes.end()) {
        NodeHandler& handler = findIter->second;
        Node* node = &handler.getNode();
        nodeAllocators.deallocate(handler.getNodeAllocatorIndex(), node);

        nodes.erase(findIter);
    }
}

void Scene::deleteNode(Node* node) {
    auto findIter = nodes.find(node->getID());
    if (findIter != nodes.end()) {
        NodeHandler& handler = findIter->second;
        node->removeAllChildren();
        nodeAllocators.deallocate(handler.getNodeAllocatorIndex(), node);

        nodes.erase(findIter);
    }
}

Node* Scene::getNode(NodeID id) {
    auto findIter = nodes.find(id);
    if (findIter == nodes.end()) {
        return nullptr;
    }

    return &(findIter->second.getNode());
}
