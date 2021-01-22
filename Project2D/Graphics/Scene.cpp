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

TransformHandler& Scene::createGraphicTransform(NodeID id) {
    size_t oldAllocatorCount = graphicTransformAllocators.size();

    PoolAllocatorVector::AllocationInfo allocationInfo = graphicTransformAllocators.allocate();
    Transform* newTransform = new (allocationInfo.allocationAddress) Transform();

    size_t newAllocatorCount = graphicTransformAllocators.size();
    if (oldAllocatorCount < newAllocatorCount) {
        graphicTransforms.reserve(newAllocatorCount * NODES_ALLOCATOR_SIZE);
    }

    auto newHandlerVal = graphicTransforms.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, *newTransform, allocationInfo.allocatorIndex));

    return newHandlerVal.first->second;
}

void Scene::deleteTransform(TransformID id) {
    auto findIter = transforms.find(id);
    if (findIter != transforms.end()) {
        TransformHandler& handler = findIter->second;
        transformAllocators.deallocate(handler.getTransformAllocatorIndex(), &handler.getTransform());

        transforms.erase(findIter);
    }
}

void Scene::deleteGraphicalTransform(TransformID id) {
    graphicMtx.lock();

    auto findIter = graphicTransforms.find(id);
    if (findIter != graphicTransforms.end()) {
        TransformHandler& handler = findIter->second;
        
        handler.decrementCounter();
        if (handler.getCounter() == 0) {
            graphicTransformAllocators.deallocate(handler.getTransformAllocatorIndex(), &handler.getTransform());

            graphicTransforms.erase(findIter);
        }
    }

    graphicMtx.unlock();
}

Node* Scene::createNode(NodeID id) {
    size_t oldAllocatorCount = nodeAllocators.size();

    PoolAllocatorVector::AllocationInfo allocationInfo = nodeAllocators.allocate();

    Transform& transform = createTransform(id);
    TransformHandler& graphicTransformHandler = createGraphicTransform(id);
    graphicTransformHandler.incrementCounter();

    Node* newNode = new (allocationInfo.allocationAddress) Node(transform, graphicTransformHandler.getTransform());

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
        deleteGraphicalTransform(id);

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
