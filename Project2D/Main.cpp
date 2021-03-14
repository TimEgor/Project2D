#include <ResourceManager/ResourceManager.h>
#include <UserInterfaces/UserInterfaces.h>
#include <UserInterfaces/Input.h>
#include <BaseGameLogic/LevelManager.h>
#include <EntityManager/Entity.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <Graphics/D3D11/D3D11.h>
#include <Graphics/D3D11/D3D11TestRenderer.h>
#include <Graphics/Scene.h>

#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds Millisecond;

int main() {
	ResourceManager& resourceManager = ResourceManager::get();
	resourceManager.init();

	UserInterfaces& userInterfaces = UserInterfaces::get();
	userInterfaces.init();

	D3D11& d3d11 = D3D11::get();
	d3d11.init();

	D3D11Renderer& d3d11Renderer = D3D11Renderer::get();
	d3d11Renderer.init();

	//

	LevelManager& levelManager = LevelManager::get();
	Level* level = levelManager.createLevel();
	Scene* scene = level->getScene();

	Entity* entity1 = level->createEntity();
	Entity* entity2 = level->createEntity(entity1);
	Entity* entity3 = level->createEntity();

	ResourceReference spriteResourceRef = resourceManager.getResourceFromArchive("testTexture.png");

	SpriteRendererEntityComponent* spriteComponent1 = (SpriteRendererEntityComponent*)(level->createEntityComponent(SpriteRendererEntityComponentType, entity1));
	spriteComponent1->setSpriteResource(spriteResourceRef);

	SpriteRendererEntityComponent* spriteComponent2 = (SpriteRendererEntityComponent*)(level->createEntityComponent(SpriteRendererEntityComponentType, entity2));
	spriteComponent2->setSpriteResource(spriteResourceRef);

	Node* node1 = scene->getNode(entity1->getID());
	Node* node2 = scene->getNode(entity2->getID());
	node2->setPositionX(1.0f);
	node2->setScaleX(0.5f);
	node2->setScaleY(0.5f);

	Input* input = userInterfaces.getInput();
	//

	Clock::time_point startTime = Clock::now();
	Clock::time_point currentTime, lastFrameStartTime;
	lastFrameStartTime = startTime;

	std::chrono::duration<float> deltaClockTime;
	float deltaTime;

	float time = 0.0f;

	MSG msg{ 0 };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, userInterfaces.getMainWindow(), 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		currentTime = Clock::now();
		deltaClockTime = currentTime - lastFrameStartTime;
		deltaTime = deltaClockTime.count();

		input->update();

		node1->setPositionX(2.0f * std::sinf(time));
		node1->setPositionY(2.0f * std::cosf(time));

		node1->setRotation(node1->getRotation() + 10.0f * deltaTime);
		node2->setRotation(node2->getRotation() + 15.0f * deltaTime);

		time += deltaTime;

		//Logic

		//if (input->isKeyDown(KeysMap::RightArrow)) {
		//	entity1Transform->setPositionX(entity1Transform->getPositionX() + 0.5f * deltaTime);
		//}

		//if (input->isKeyDown(KeysMap::LeftArrow)) {
		//	entity1Transform->setPositionX(entity1Transform->getPositionX() - 0.5f * deltaTime);
		//}

		//Rendering
		RenderingData renderingData = level->getRenderingData();
		d3d11Renderer.draw(renderingData);

		lastFrameStartTime = currentTime;
		Millisecond frameTime = std::chrono::duration_cast<Millisecond>(Clock::now() - currentTime);
	}

	return 0;
}