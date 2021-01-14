#include <ResourceManager/ResourceManager.h>

#include <EntityManager/EntityManager.h>

int main() {
	ResourceManager& resourceManager = ResourceManager::get();
	resourceManager.init();

	EntityManager entityManager;
	entityManager.init();

	Entity* entity1 = entityManager.createEntity();
	Entity* entity2 = entityManager.createEntity();
	Entity* entity3 = entityManager.createEntity();

	entityManager.deleteEntity(entity2);

	return 0;
}