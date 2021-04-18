#pragma once

#include <Graphics/NodeHandler.h>
#include <Graphics/TransformHandler.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>
#include <MemoryManager/Allocators/PoolAllocator.h>

#include <unordered_map>
#include <mutex>

#define SCENE_NODES_ALLOCATOR_SIZE 4096
#define CANVAS_NODES_ALLOCATOR_SIZE 1024

class Level;

class Scene final {
	typedef PoolAllocatorVector<PoolAllocator> Allocators;

private:
	std::unordered_map<NodeID, NodeHandler> nodes;
	std::unordered_map<TransformID, TransformHandler> transforms;
	Allocators sceneNodeAllocators;
	Allocators canvasNodeAllocators;
	Allocators sceneTransformAllocators;
	Allocators canvasTransformAllocators;
	Allocators worldTransformsMatrixAllocators;

	Level* level;

	SceneTransform* createSceneTransform(NodeID id);
	CanvasTransform* createCanvasTransform(NodeID id);
	void deleteTransform(TransformID id);

	void deleteChildrenNodes(Node* node);

public:
	Scene() = default;
	~Scene() { release(); }

	bool init(Level* level);
	void release();

	SceneNode* createSceneNode(NodeID id);
	SceneNode* createSceneNode(NodeID id, NodeID parentID);
	SceneNode* createSceneNode(NodeID id, Node* parent);
	CanvasNode* createCanvasNode(NodeID id);
	CanvasNode* createCanvasNode(NodeID id, NodeID parentID);
	CanvasNode* createCanvasNode(NodeID id, Node* parent);
	void deleteNode(NodeID id);
	void deleteNode(Node* node);

	Node* getNode(NodeID id);
	Transform* getTransform(TransformID id);
};