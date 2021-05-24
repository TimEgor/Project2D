#include <Core.h>

#include <LevelManager/LevelManager.h>
#include <EntityManager/Entity.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <Graphics/Node.h>
#include <ResourceManager/ResourceManager.h>

#include <Windows.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int show) {
	Core& core = Core::get();

	core.init();

	Level* level = LevelManager::get().createLevel();

	Entity* entity = level->createSceneEntity();
	SpriteRendererEntityComponent* spriteComponent = level->createEntityComponent<SpriteRendererEntityComponent>(entity, ResourceManager::get().getResourceFromArchive("testTexture.png"));

	SceneNode* node = (SceneNode*)(entity->getNode());
	node->setPositionX(2.0f);

	core.run();
	core.release();
}