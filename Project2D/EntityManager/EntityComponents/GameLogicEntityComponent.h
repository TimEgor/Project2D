#pragma once

#include <EntityManager/EntityComponent.h>

class GameLogicEntityComponent : public EntityComponent {
public:
	GameLogicEntityComponent() = default;

	virtual void update(float deltaTime) = 0;
};