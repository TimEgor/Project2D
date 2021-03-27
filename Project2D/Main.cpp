#include <ResourceManager/ResourceManager.h>
#include <UserInterfaces/UserInterfaces.h>
#include <UserInterfaces/Input.h>
#include <BaseGameLogic/LevelManager.h>
#include <BaseGameLogic/UICanvasManager.h>
#include <EntityManager/Entity.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <Graphics/NodeManager.h>
#include <Graphics/D3D11/D3D11.h>
#include <Graphics/D3D11/D3D11TestRenderer.h>

#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds Millisecond;

int main() {
	// Init 

	ResourceManager& resourceManager = ResourceManager::get();
	resourceManager.init();

	UserInterfaces& userInterfaces = UserInterfaces::get();
	userInterfaces.init();

	D3D11& d3d11 = D3D11::get();
	d3d11.init();

	D3D11Renderer& d3d11Renderer = D3D11Renderer::get();
	d3d11Renderer.init();

	//

	ResourceReference spriteResourceRef = resourceManager.getResourceFromArchive("testTexture.png");
	Input* input = userInterfaces.getInput();

	// Scene

	LevelManager& levelManager = LevelManager::get();
	Level* sceneSpace = levelManager.createLevel();
	SceneNodeManager* sceneNodeManager = sceneSpace->getNodeManager();

	Entity* sceneEntity1 = sceneSpace->createEntity();
	Entity* sceneEntity2 = sceneSpace->createEntity(sceneEntity1);

	SpriteRendererEntityComponent* sceneSpriteComponent1 = (SpriteRendererEntityComponent*)(sceneSpace->createEntityComponent(SpriteRendererEntityComponentType, sceneEntity1));
	sceneSpriteComponent1->setSpriteResource(spriteResourceRef);

	SpriteRendererEntityComponent* sceneSpriteComponent2 = (SpriteRendererEntityComponent*)(sceneSpace->createEntityComponent(SpriteRendererEntityComponentType, sceneEntity2));
	sceneSpriteComponent2->setSpriteResource(spriteResourceRef);

	SceneNodeManager::NodeType* sceneNode1 = sceneNodeManager->getNode(sceneEntity1->getID());
	SceneNodeManager::NodeType* sceneNode2 = sceneNodeManager->getNode(sceneEntity2->getID());
	sceneNode2->setPositionX(1.0f);
	sceneNode2->setScaleX(0.5f);
	sceneNode2->setScaleY(0.5f);

	// UI

	UICanvasManager& canvasManager = UICanvasManager::get();
	UICanvas* canvas = canvasManager.createCanvas();
	CanvasNodeManager* canvasNodeManager = canvas->getNodeManager();

	Entity* uiEntity = canvas->createEntity(1);

	SpriteRendererEntityComponent* uiSpriteComponent = (SpriteRendererEntityComponent*)(canvas->createEntityComponent(SpriteRendererEntityComponentType, uiEntity));
	uiSpriteComponent->setSpriteResource(spriteResourceRef);

	CanvasNode* uiNode = canvasNodeManager->getNode(uiEntity->getID());
	uiNode->setPivotX(0.0f);
	//uiNode->setPivotY(0.0f);
	uiNode->setPositionY(400.0f);
	uiNode->setPositionX(50.0f);
	uiNode->setScaleX(2.0f);
	uiNode->setScaleY(2.0f);

	// Loop

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

		sceneNode1->setPositionX(2.0f * std::sinf(time));
		sceneNode1->setPositionY(2.0f * std::cosf(time));

		sceneNode1->setRotation(sceneNode1->getRotation() + 10.0f * deltaTime);
		sceneNode2->setRotation(sceneNode2->getRotation() + 15.0f * deltaTime);

		time += deltaTime;

		//Rendering
		d3d11Renderer.beginDrawing();

		SceneSpaceRenderingData sceneRenderingData = sceneSpace->getRenderingData();
		d3d11Renderer.draw(sceneRenderingData);

		CanvasSpaceRenderingData canvasRenderingData = canvas->getRenderingData();
		d3d11Renderer.draw(canvasRenderingData);

		d3d11Renderer.endDrawing();

		lastFrameStartTime = currentTime;
		Millisecond frameTime = std::chrono::duration_cast<Millisecond>(Clock::now() - currentTime);
	}

	return 0;
}