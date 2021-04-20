#include <ResourceManager/ResourceManager.h>
#include <UserInterfaces/UserInterfaces.h>
#include <UserInterfaces/Input.h>
#include <BaseGameLogic/LevelManager.h>
#include <EntityManager/Entity.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <EntityManager/EntityComponents/Canvas/CanvasSpriteRendererEntityComponent.h>
#include <Graphics/D3D11/D3D11.h>
#include <Graphics/D3D11/D3D11Renderer.h>
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

	Input* input = userInterfaces.getInput();

	ResourceReference spriteResourceRef = resourceManager.getResourceFromArchive("testTexture.png");

	//Scene
	Entity* sceneEntity1 = level->createSceneEntity();
	Entity* sceneEntity2 = level->createSceneEntity(sceneEntity1);
	Entity* sceneEntity3 = level->createSceneEntity();

	SpriteRendererEntityComponent* spriteSceneComponent1 = (SpriteRendererEntityComponent*)(level->createEntityComponent(SpriteRendererEntityComponentType, sceneEntity1));
	spriteSceneComponent1->setSpriteResource(spriteResourceRef);

	SpriteRendererEntityComponent* spriteSceneComponent2 = (SpriteRendererEntityComponent*)(level->createEntityComponent(SpriteRendererEntityComponentType, sceneEntity2));
	spriteSceneComponent2->setSpriteResource(spriteResourceRef);

	SceneNode* node1 = (SceneNode*)(scene->getNode(sceneEntity1->getID()));
	SceneNode* node2 = (SceneNode*)(scene->getNode(sceneEntity2->getID()));
	node2->setPositionX(0.5f);
	node2->setScaleX(0.5f);
	node2->setScaleY(0.5f);
	
	//Canvas
	Entity* canvas = level->createCanvasEntity();
	CanvasNode* canvasNode = (CanvasNode*)(scene->getNode(canvas->getID()));
	canvasNode->setWidth(800.0f);
	canvasNode->setHeight(800.0f);
	canvasNode->setPivotX(0.5f);
	canvasNode->setPivotY(0.5f);

	Entity* canvasEntity1 = level->createCanvasEntity(canvas);
	CanvasSpriteRendererEntityComponent* spriteCanvasComponent1 = (CanvasSpriteRendererEntityComponent*)(level->createEntityComponent(CanvasSpriteRendererEntityComponentType, canvasEntity1));
	spriteCanvasComponent1->setSpriteResource(spriteResourceRef);

	CanvasNode* canvasNode1 = (CanvasNode*)(scene->getNode(canvasEntity1->getID()));
	canvasNode1->setWidth(100.0f);
	canvasNode1->setHeight(100.0f);
	canvasNode1->setPivotX(0.0f);
	canvasNode1->setPivotY(0.0f);
	canvasNode1->setAnchorX(0.5f);
	canvasNode1->setAnchorY(0.5f);
	//canvasNode1->setPositionX(500.0f);
	//canvasNode1->setPositionY(500.0f);

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

		//Logic
		node1->setPositionX(2.0f * std::sinf(time));
		node1->setPositionY(2.0f * std::cosf(time));

		node1->setRotation(node1->getRotation() + 10.0f * deltaTime);
		node2->setRotation(node2->getRotation() + 15.0f * deltaTime);

		time += deltaTime;

		//Rendering
		d3d11Renderer.beginDrawing();

		RenderingData renderingData = level->getRenderingData();
		d3d11Renderer.draw(renderingData);

		d3d11Renderer.endDrawing();

		lastFrameStartTime = currentTime;
		Millisecond frameTime = std::chrono::duration_cast<Millisecond>(Clock::now() - currentTime);
	}

	return 0;
}