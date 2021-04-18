#include "Scene.h"

#include <MemoryManager/MemoryManager.h>
#include <BaseGameLogic/Level.h>

bool Scene::init(Level* level) {
    nodes.reserve(SCENE_NODES_ALLOCATOR_SIZE + CANVAS_NODES_ALLOCATOR_SIZE);
    if (!sceneNodeAllocators.init(MemoryManager::get().getDefaultHeap(), sizeof(SceneNode), SCENE_NODES_ALLOCATOR_SIZE)) {
        release();
        return false;
    }

    if (!canvasNodeAllocators.init(MemoryManager::get().getDefaultHeap(), sizeof(CanvasNode), CANVAS_NODES_ALLOCATOR_SIZE)) {
        release();
        return false;
    }

    if (!sceneTransformAllocators.init(MemoryManager::get().getDefaultHeap(), sizeof(SceneTransform), SCENE_NODES_ALLOCATOR_SIZE)) {
        release();
        return false;
    }

    if (!canvasTransformAllocators.init(MemoryManager::get().getDefaultHeap(), sizeof(CanvasTransform), CANVAS_NODES_ALLOCATOR_SIZE)) {
        release();
        return false;
    }


    if (!worldTransformsMatrixAllocators.init(MemoryManager::get().getDefaultHeap(), sizeof(TransformMatrix), SCENE_NODES_ALLOCATOR_SIZE + CANVAS_NODES_ALLOCATOR_SIZE)) {
        release();
        return false;
    }

    return true;
}

void Scene::release() {
    nodes = std::unordered_map<NodeID, NodeHandler>();
    sceneNodeAllocators.release();
    canvasNodeAllocators.release();
    sceneTransformAllocators.release();
    canvasTransformAllocators.release();
}

SceneTransform* Scene::createSceneTransform(NodeID id) {
    size_t oldAllocatorCount = sceneTransformAllocators.size();

    Allocators::AllocationInfo transformAllocationInfo = sceneTransformAllocators.allocate();
    Allocators::AllocationInfo matrixAllocationInfo = worldTransformsMatrixAllocators.allocate();
    TransformMatrix* matrix = new (matrixAllocationInfo.allocationAddress) TransformMatrix();
    SceneTransform* newTransform = new (transformAllocationInfo.allocationAddress) SceneTransform(matrix);

    size_t newAllocatorCount = sceneTransformAllocators.size();
    if (oldAllocatorCount < newAllocatorCount) {
        transforms.reserve(newAllocatorCount * SCENE_NODES_ALLOCATOR_SIZE);
    }

    transforms.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, newTransform, transformAllocationInfo.allocatorID));

    return newTransform;
}

CanvasTransform* Scene::createCanvasTransform(NodeID id) {
    size_t oldAllocatorCount = canvasTransformAllocators.size();

    Allocators::AllocationInfo transformAllocationInfo = canvasTransformAllocators.allocate();
    Allocators::AllocationInfo matrixAllocationInfo = worldTransformsMatrixAllocators.allocate();
    TransformMatrix* matrix = new (matrixAllocationInfo.allocationAddress) TransformMatrix();
    CanvasTransform* newTransform = new (transformAllocationInfo.allocationAddress) CanvasTransform(matrix);

    size_t newAllocatorCount = sceneTransformAllocators.size();
    if (oldAllocatorCount < newAllocatorCount) {
        transforms.reserve(newAllocatorCount * CANVAS_NODES_ALLOCATOR_SIZE);
    }

    transforms.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, newTransform, transformAllocationInfo.allocatorID));

    return newTransform;
}

void Scene::deleteTransform(TransformID id) {
    auto findIter = transforms.find(id);
    if (findIter != transforms.end()) {
        TransformHandler& handler = findIter->second;
        Transform* transform = handler.getTransform();

        worldTransformsMatrixAllocators.deallocate(handler.getTransformAllocatorID(), transform->getWorldTransformMatrix());

        if (transform->getTransformType() == SceneTransformType) {
            sceneTransformAllocators.deallocate(handler.getTransformAllocatorID(), transform);
        }
        else if (transform->getTransformType() == CanvasTransformType) {
            canvasTransformAllocators.deallocate(handler.getTransformAllocatorID(), transform);
        }

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

SceneNode* Scene::createSceneNode(NodeID id) {
    size_t oldAllocatorCount = sceneNodeAllocators.size();

    Allocators::AllocationInfo allocationInfo = sceneNodeAllocators.allocate();

    SceneTransform* transform = createSceneTransform(id);

    SceneNode* newNode = new (allocationInfo.allocationAddress) SceneNode(transform);

    size_t newAllocatorCount = sceneNodeAllocators.size();
    if (oldAllocatorCount < newAllocatorCount) {
        nodes.reserve(newAllocatorCount * SCENE_NODES_ALLOCATOR_SIZE);
    }

    nodes.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, newNode, allocationInfo.allocatorID));

    return newNode;
}

SceneNode* Scene::createSceneNode(NodeID id, NodeID parentID) {
    SceneNode* newNode = nullptr;

    auto nodeIter = nodes.find(parentID);
    if (nodeIter != nodes.end()) {
        NodeHandler& handler = nodeIter->second;
        Node* parentNode = handler.getNode();

        newNode = createSceneNode(id);

        parentNode->addChild(newNode);
    }

    return newNode;
}

SceneNode* Scene::createSceneNode(NodeID id, Node* parent) {
    return createSceneNode(id, parent->getID());
}

CanvasNode* Scene::createCanvasNode(NodeID id) {
    size_t oldAllocatorCount = sceneNodeAllocators.size();

    Allocators::AllocationInfo allocationInfo = canvasNodeAllocators.allocate();

    CanvasTransform* transform = createCanvasTransform(id);

    CanvasNode* newNode = new (allocationInfo.allocationAddress) CanvasNode(transform);

    size_t newAllocatorCount = canvasNodeAllocators.size();
    if (oldAllocatorCount < newAllocatorCount) {
        nodes.reserve(newAllocatorCount * CANVAS_NODES_ALLOCATOR_SIZE);
    }

    nodes.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, newNode, allocationInfo.allocatorID));

    return newNode;
}

CanvasNode* Scene::createCanvasNode(NodeID id, NodeID parentID) {
    CanvasNode* newNode = nullptr;

    auto nodeIter = nodes.find(parentID);
    if (nodeIter != nodes.end()) {
        NodeHandler& handler = nodeIter->second;
        Node* parentNode = handler.getNode();

        newNode = createCanvasNode(id);

        parentNode->addChild(newNode);
    }

    return newNode;
}

CanvasNode* Scene::createCanvasNode(NodeID id, Node* parent) {
    return createCanvasNode(id, parent->getID());
}

void Scene::deleteNode(NodeID id) {
    auto nodeIter = nodes.find(id);
    if (nodeIter != nodes.end()) {
        NodeHandler& handler = nodeIter->second;
        Node* node = handler.getNode();

        deleteTransform(nodeIter->first);
        deleteChildrenNodes(node);

        if (node->getNodeType() == SceneNodeType) {
            sceneNodeAllocators.deallocate(handler.getNodeAllocatorID(), node);
        }
        else if (node->getNodeType() == CanvasNodeType) {
            canvasNodeAllocators.deallocate(handler.getNodeAllocatorID(), node);
        }

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

    return findIter->second.getNode();
}

Transform* Scene::getTransform(TransformID id) {
    auto findIter = transforms.find(id);
    if (findIter == transforms.end()) {
        return nullptr;
    }

    return findIter->second.getTransform();
}
