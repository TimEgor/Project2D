#include "Scene.h"

#include <MemoryManager/MemoryManager.h>
#include <BaseGameLogic/Level.h>

bool Scene::init(Level* level) {
    nodes.reserve(NODES_ALLOCATOR_SIZE);
    if (!nodeAllocators.init(MemoryManager::get().getDefaultHeap(), sizeof(Node), NODES_ALLOCATOR_SIZE)) {
        release();
        return false;
    }

    if (!transformAllocators.init(MemoryManager::get().getDefaultHeap(), sizeof(Transform), NODES_ALLOCATOR_SIZE)) {
        release();
        return false;
    }

    return true;
}

void Scene::release() {
    nodes = std::unordered_map<NodeID, NodeHandler>();
    nodeAllocators.release();
}

Transform& Scene::createTransform(NodeID id) {
    size_t oldAllocatorCount = transformAllocators.size();

    Allocators::AllocationInfo allocationInfo = transformAllocators.allocate();
    Transform* newTransform = new (allocationInfo.allocationAddress) Transform();

    size_t newAllocatorCount = transformAllocators.size();
    if (oldAllocatorCount < newAllocatorCount) {
        transforms.reserve(newAllocatorCount * NODES_ALLOCATOR_SIZE);
    }

    transforms.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, *newTransform, allocationInfo.allocatorID));

    return *newTransform;
}

void Scene::deleteTransform(TransformID id) {
    auto findIter = transforms.find(id);
    if (findIter != transforms.end()) {
        TransformHandler& handler = findIter->second;
        transformAllocators.deallocate(handler.getTransformAllocatorID(), &handler.getTransform());

        transforms.erase(findIter);
    }
}

void Scene::deleteChildrenNodes(Node* node) {
    auto childrens = node->getChildren();
    for (auto child : childrens) {
        NodeID childID = child->getID();
        level->deleteEntityWithoutNode(childID);
        deleteNode(childID);
    }
}

Node* Scene::createNode(NodeID id) {
    size_t oldAllocatorCount = nodeAllocators.size();

    Allocators::AllocationInfo allocationInfo = nodeAllocators.allocate();

    Transform& transform = createTransform(id);

    Node* newNode = new (allocationInfo.allocationAddress) Node(transform);

    size_t newAllocatorCount = nodeAllocators.size();
    if (oldAllocatorCount < newAllocatorCount) {
        nodes.reserve(newAllocatorCount * NODES_ALLOCATOR_SIZE);
    }

    nodes.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, *newNode, allocationInfo.allocatorID));

    return newNode;
}

void Scene::deleteNode(NodeID id) {
    auto nodeIter = nodes.find(id);
    if (nodeIter != nodes.end()) {
        NodeHandler& handler = nodeIter->second;
        Node* node = &handler.getNode();

        deleteTransform(nodeIter->first);
        deleteChildrenNodes(node);

        nodeAllocators.deallocate(handler.getNodeAllocatorID(), node);

        nodes.erase(nodeIter);
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

Transform* Scene::getTransform(TransformID id) {
    auto findIter = transforms.find(id);
    if (findIter == transforms.end()) {
        return nullptr;
    }

    return &(findIter->second.getTransform());
}
