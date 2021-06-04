#pragma once

class SpriteRendererEntityComponent;
class CanvasSpriteRendererEntityComponent;
class CanvasLabelEntityComponent;

enum EntityComponentType {
	SpriteRendererEntityComponentType,
	CanvasSpriteRendererEntityComponentType,
	CanvasLabelEntityComponentType,
	CppGameLogicEntityComponentType
};