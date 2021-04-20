#pragma once

#include <cstdint>

//Node
typedef uint32_t NodeID;

enum NodeType {
	SceneNodeType,
	CanvasNodeType
};

class Node;
class SceneNode;
class CanvasNode;

//Scene
class Scene;

//Transform
typedef uint32_t TransformID;

enum TransformType {
	SceneTransformType,
	CanvasTransformType
};

class Transform;
class SceneTransform;
class CanvasTransform;