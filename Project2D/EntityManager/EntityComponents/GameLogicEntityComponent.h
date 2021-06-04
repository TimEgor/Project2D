#pragma once

#include <EntityManager/EntityComponent.h>

class GameLogicEntityComponent : public EntityComponent {
public:
	GameLogicEntityComponent(EntityComponentHandler* handler) : EntityComponent(handler) {}

	virtual void update(float deltaTime) = 0;
};