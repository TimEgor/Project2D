#include <ResourceManager/ResourceManager.h>
#include <UserInterfaces/UserInterfaces.h>
#include <BaseGameLogic/LevelManager.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <Graphics/D3D11/D3D11.h>
#include <Graphics/D3D11/D3D11TestRenderer.h>

int main() {
	ResourceManager& resourceManager = ResourceManager::get();
	resourceManager.init();

	UserInterfaces& userInterfaces = UserInterfaces::get();
	userInterfaces.init();

	D3D11& d3d11 = D3D11::get();
	d3d11.init();

	D3D11TestRenderer& d3d11TestRenderer = D3D11TestRenderer::get();
	d3d11TestRenderer.init();

	LevelManager& levelManager = LevelManager::get();
	Level* level = levelManager.createLevel();

	Entity* entity1 = level->createEntity();
	Entity* entity2 = level->createEntity();
	Entity* entity3 = level->createEntity();

	ResourceReference spriteResourceRef = resourceManager.getResourceFromArchive("testtexture.png");

	SpriteRendererEntityComponent* spriteComponent1 = (SpriteRendererEntityComponent*)(level->createEntityComponent(SpriteRendererEntityComponentType, entity1));
	spriteComponent1->setSpriteResource(spriteResourceRef);

	SpriteRendererEntityComponent* spriteComponent2 = (SpriteRendererEntityComponent*)(level->createEntityComponent(SpriteRendererEntityComponentType, entity2));
	spriteComponent2->setSpriteResource(spriteResourceRef);

	MSG msg{0};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, userInterfaces.getMainWindow(), 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//Logic
		d3d11TestRenderer.render();
	}

	return 0;
}