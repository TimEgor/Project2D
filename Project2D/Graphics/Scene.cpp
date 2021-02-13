#include "Scene.h"

#include <MemoryManager/MemoryManager.h>

bool Scene::init() {
    nodes.reserve(NODES_ALLOCATOR_SIZE);
    if (!nodeAllocators.init(MemoryManager::get().getDefaultHeap(), sizeof(Node), NODES_ALLOCATOR_SIZE)) {
        release();
        return false;
    }
}

void Scene::release() {
    nodes = std::unordered_map<NodeID, NodeHandler>();
    nodeAllocators.release();
}

Transform& Scene::createTransform(NodeID id) {
    size_t oldAllocatorCount = transformAllocators.size();

    PoolAllocatorVector::AllocationInfo allocationInfo = transformAllocators.allocate();
    Transform* newTransform = new (allocationInfo.allocationAddress) Transform();

    size_t newAllocatorCount = transformAllocators.size();
    if (oldAllocatorCount < newAllocatorCount) {
        transforms.reserve(newAllocatorCount * NODES_ALLOCATOR_SIZE);
    }

    transforms.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, *newTransform, allocationInfo.allocatorIndex));

    return *newTransform;
}

void Scene::deleteTransform(TransformID id) {
    auto findIter = transforms.find(id);
    if (findIter != transforms.end()) {
        TransformHandler& handler = findIter->second;
        transformAllocators.deallocate(handler.getTransformAllocatorIndex(), &handler.getTransform());

        transforms.erase(findIter);
    }
}

Node* Scene::createNode(NodeID id) {
    size_t oldAllocatorCount = nodeAllocators.size();

    PoolAllocatorVector::AllocationInfo allocationInfo = nodeAllocators.allocate();

    Transform& transform = createTransform(id);

    Node* newNode = new (allocationInfo.allocationAddress) Node(transform);

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

        deleteTransform(id);

        nodeAllocators.deallocate(handler.getNodeAllocatorIndex(), node);

        nodes.erase(findIter);
    }
}

void Scene::deleteNode(Node* node) {
    deleteNode(node->getID());
}

Node* Scene::getNode(NodeID id) {
    auto findIter = nodes.find(id);
    if (findIter == nodes.end()) {
        return nullptr;
    }

    return &(findIter->second.getNode());
}
