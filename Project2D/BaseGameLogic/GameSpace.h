#pragma once

#include <BaseGameLogic/BaseGameSpace.h>
#include <EntityManager/EntityManager.h>
#include <EntityManager/EntityComponentManager.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <Graphics/NodeManager.h>
#include <Graphics/RenderingData.h>
#include <ResourceManager/ResourceManager.h>

#include <cassert>

template <typename NodeManagerType>
class GameSpace final : public BaseGameSpace {
private:
	NodeManagerType* nodeManager;

public:
	GameSpace(GameSpaceID spaceID) : BaseGameSpace(spaceID), nodeManager(nullptr) {}
	~GameSpace() { release(); }

	virtual bool init() override;
	virtual void release() override;

	virtual Entity* createEntity() override;
	virtual Entity* createEntity(Entity* parent) override;
	virtual Entity* createEntity(EntityID parentID) override;
	virtual void deleteEntity(Entity* entity) override;
	virtual void deleteEntity(EntityID id) override;

	NodeManagerRenderingData<NodeManagerType> getRenderingData();

	NodeManagerType* getNodeManager() { return nodeManager; }
};

typedef GameSpace<CanvasNodeManager> CanvasGameSpace;
typedef GameSpace<SceneNodeManager> SceneGameSpace;

template<typename NodeManagerType>
inline bool GameSpace<NodeManagerType>::init() {
	if (!BaseGameSpace::init()) {
		return false;
	}

	nodeManager = new NodeManagerType();

	if (!nodeManager->init(this)) {
		release();
		return false;
	}

	return false;
}

template<typename NodeManagerType>
inline void GameSpace<NodeManagerType>::release() {
	if (nodeManager) {
		nodeManager->release();
		delete nodeManager;
		nodeManager = nullptr;
	}

	BaseGameSpace::release();
}

template<typename NodeManagerType>
inline Entity* GameSpace<NodeManagerType>::createEntity() {
	Entity* entity = entityManager->createEntity();
	nodeManager->createNode(entity->getID());

	return entity;
}

template<typename NodeManagerType>
inline Entity* GameSpace<NodeManagerType>::createEntity(Entity* parent) {
	return createEntity(parent->getID());
}

template<typename NodeManagerType>
inline Entity* GameSpace<NodeManagerType>::createEntity(EntityID parentID) {
	Entity* entity = entityManager->createEntity();
	nodeManager->createNode(entity->getID(), parentID);

	return entity;
}

template<typename NodeManagerType>
inline void GameSpace<NodeManagerType>::deleteEntity(Entity* entity) {
	deleteEntityComponentsFromEntity(entity);

	nodeManager->deleteNode(entity->getID());
	entityManager->deleteEntity(entity);
}

template<typename NodeManagerType>
inline void GameSpace<NodeManagerType>::deleteEntity(EntityID id) {
	Entity* entity = entityManager->getEntity(id);
	deleteEntity(entity);
}

template<typename NodeManagerType>
inline NodeManagerRenderingData<NodeManagerType> GameSpace<NodeManagerType>::getRenderingData() {
	NodeManagerRenderingData<NodeManagerType> renderingData;
	renderingData.setNodeManager(nodeManager);
	renderingData.setRenderingOrder(renderingOrder);

	renderingOrder->clear();

	size_t spritesNum = entityComponentManager->getEntityComponentsNum(SpriteRendererEntityComponentType);
	if (spritesNum != 0) {
		renderingOrder->preReSize(spritesNum);

		if (auto spritesAllocators = entityComponentManager->getEntityComponents(SpriteRendererEntityComponentType)) {
			size_t allocatorsNum = spritesAllocators->size();
			for (size_t allocatorIndex = 0; allocatorIndex < allocatorsNum; ++allocatorIndex) {
				const ArrayPoolAllocator& allocator = (*spritesAllocators)[allocatorIndex];
				size_t allocatorSize = allocator.size();

				for (size_t componentIndex = 0; componentIndex < allocatorSize; ++componentIndex) {
					const SpriteRendererEntityComponent& component = allocator.getElement<SpriteRendererEntityComponent>(componentIndex);

					ResourceReference spriteResource = component.getSpriteResource();
					if (!spriteResource.isNull()) {
						EntityID id = component.getParentID();

						Node* node = nodeManager->getNode(id);
						node->updateTransform();

						TransformMatrix* worldTransform = node->getTransform()->getWorldTransformMatrix();

						renderingOrder->pushNode(id, component.getMaterialResource(), spriteResource, worldTransform);
					}
				}
			}
		}
	}

	return renderingData;
}
