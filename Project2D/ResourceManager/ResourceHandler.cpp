#include "ResourceHandler.h"

#include <ResourceManager/ResourceManager.h>

void ResourceHandler::selfDestroy() {
	ResourceManager::get().unloadResource(*this);
}
