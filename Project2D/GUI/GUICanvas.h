#pragma once

#include <EntityManager/EntityManagerTypes.h>
#include <EntityManager/EntityComponentType.h>
#include <Graphics/RenderingData.h>

#include <cstdint>

class EntityManager;
class EntityComponentManager;

typedef uint32_t GUICanvasID;

class GUICanvas final {
private:
	EntityManager* entityManager;
	EntityComponentManager* entityComponentManager;

	RenderingOrder* renderingOrder;

	GUICanvasID canvasID;

public:
	GUICanvas(GUICanvasID canvasID) : entityManager(nullptr), entityComponentManager(nullptr), canvasID(canvasID) {}
	GUICanvas(const GUICanvas&) = delete;
	~GUICanvas() { release(); }

	bool init();
	void release();

	GUICanvasID getID() const { return canvasID; }

	RenderingData getRenderingData();

	EntityManager* getEntityManager() { return entityManager; }
	EntityComponentManager* getEntityComponentManager() { return entityComponentManager; }
};