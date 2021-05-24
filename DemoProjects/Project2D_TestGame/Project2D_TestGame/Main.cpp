#include <Core.h>

#include <BaseGameLogic/LevelManager.h>
#include <EntityManager/Entity.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <ResourceManager/ResourceManager.h>

#include <Windows.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int show) {
	Core& core = Core::get();

	core.init();

	Level* level = LevelManager::get().createLevel();

	Entity* entity = level->createSceneEntity();
	SpriteRendererEntityComponent* spriteComponent = level->createEntityComponent<SpriteRendererEntityComponent>(entity, ResourceManager::get().getResourceFromArchive("testTexture.png"));

	core.run();
	core.release();
}