#include "EntityComponent.h"

#include <EntityManager/EntityComponentHandler.h>

EntityComponentID EntityComponent::getID() const {
    if (handler) {
        return handler->getID();
    }

    return 0;
}
