#include "EntityComponentHandler.h"

#include <EntityManager/EntityComponentManager.h>
#include <EntityManager/EntityComponentReference.h>
#include <LevelManager/Level.h>

EntityComponentReference EntityComponentHandler::getReference() {
	if (!reference) {
		reference = level->getEntityComponentManager()->createReference(this);
	}

	return reference;
}
