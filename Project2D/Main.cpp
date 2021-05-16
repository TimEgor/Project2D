#include <ResourceManager/ResourceManager.h>
#include <UserInterfaces/UserInterfaces.h>
#include <UserInterfaces/Input.h>
#include <BaseGameLogic/LevelManager.h>
#include <EntityManager/Entity.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <EntityManager/EntityComponents/Canvas/CanvasSpriteRendererEntityComponent.h>
#include <EntityManager/EntityComponents/Canvas/CanvasLabelEntityComponent.h>
#include <Graphics/D3D11/D3D11.h>
#include <Graphics/D3D11/D3D11Renderer.h>
#include <Graphics/Scene.h>
#include <Graphics/FontManager.h>

#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds Millisecond;

int main() {
	srand(time(0));

	ResourceManager& resourceManager = ResourceManager::get();
	resourceManager.init();

	UserInterfaces& userInterfaces = UserInterfaces::get();
	userInterfaces.init();

	D3D11& d3d11 = D3D11::get();
	d3d11.init();

	D3D11Renderer& d3d11Renderer = D3D11Renderer::get();
	d3d11Renderer.init();

	FontManager& fontManager = FontManager::get();
	fontManager.init();
	fontManager.addFont("Fonts/Arial");

	//

	LevelManager& levelManager = LevelManager::get();
	Level* level = levelManager.createLevel();
	Scene* scene = level->getScene();

	Input* input = userInterfaces.getInput();

	ResourceReference testSpriteResourceRef = resourceManager.getResourceFromArchive("testTexture.png");
	ResourceReference squareSpriteResourceRef = resourceManager.getResourceFromArchive("square.png");
	ResourceReference circleSpriteResourceRef = resourceManager.getResourceFromArchive("circle.png");

	std::vector<ResourceReference> sprites = { testSpriteResourceRef, squareSpriteResourceRef, circleSpriteResourceRef };

	//Scene
	Entity* sceneEntity1 = level->createSceneEntity();
	Entity* sceneEntity2 = level->createSceneEntity(sceneEntity1);
	Entity* sceneEntity3 = level->createSceneEntity();

	SpriteRendererEntityComponent* spriteSceneComponent1 = (SpriteRendererEntityComponent*)(level->createEntityComponent(SpriteRendererEntityComponentType, sceneEntity1));
	spriteSceneComponent1->setSpriteResource(testSpriteResourceRef);

	SpriteRendererEntityComponent* spriteSceneComponent2 = (SpriteRendererEntityComponent*)(level->createEntityComponent(SpriteRendererEntityComponentType, sceneEntity2));
	spriteSceneComponent2->setSpriteResource(testSpriteResourceRef);

	SceneNode* node1 = (SceneNode*)(scene->getNode(sceneEntity1->getID()));
	SceneNode* node2 = (SceneNode*)(scene->getNode(sceneEntity2->getID()));
	node2->setPositionX(0.5f);
	node2->setScaleX(0.5f);
	node2->setScaleY(0.5f);
	node2->setDepth(1.0f);
	
	//Canvas
	Entity* canvas = level->createCanvasEntity();
	CanvasNode* canvasNode = (CanvasNode*)(scene->getNode(canvas->getID()));
	canvasNode->setWidth(800.0f);
	canvasNode->setHeight(800.0f);



	for (size_t i = 0; i < 4000; ++i) {
		Entity* canvasEntity2 = level->createCanvasEntity(canvas);
		CanvasSpriteRendererEntityComponent* spriteCanvasComponent2 = (CanvasSpriteRendererEntityComponent*)(level->createEntityComponent(CanvasSpriteRendererEntityComponentType, canvasEntity2));
		spriteCanvasComponent2->setSpriteResource(sprites[rand() % 3]);

		CanvasNode* canvasNode2 = (CanvasNode*)(scene->getNode(canvasEntity2->getID()));

		int size = (rand() % 300) + 100;
		canvasNode2->setWidth(size);
		canvasNode2->setHeight(size);
		canvasNode2->setDepth(rand() % 20);
		canvasNode2->setPivotX(0.5f);
		canvasNode2->setPivotY(0.5f);
		canvasNode2->setAnchorX(0.0f);
		canvasNode2->setAnchorY(0.0f);
		canvasNode2->setPositionX(rand() % 800);
		canvasNode2->setPositionY(rand() % 800);
	}

	Entity* canvasEntity3 = level->createCanvasEntity(canvas);
	CanvasLabelEntityComponent* labelCanvasComponent = (CanvasLabelEntityComponent*)(level->createEntityComponent(CanvasLabelEntityComponentType, canvasEntity3));
	labelCanvasComponent->setText("Hello");
	CanvasNode* canvasNode3 = (CanvasNode*)(canvasEntity3->getNode());
	canvasNode3->setWidth(100.0f);
	canvasNode3->setHeight(100.0f);
	canvasNode3->setPivotX(0.0f);
	canvasNode3->setPivotY(0.0f);
	canvasNode3->setAnchorX(0.0f);
	canvasNode3->setAnchorY(0.0f);
	canvasNode3->setPositionX(50.0f);
	canvasNode3->setPositionY(50.0f);

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
		RenderingData renderingData = level->getRenderingData();
		renderingData.getSceneRedneringOrder()->sort();
		renderingData.getCanvasRedneringOrder()->sort();

		d3d11Renderer.beginDrawing();
		d3d11Renderer.draw(renderingData);
		d3d11Renderer.endDrawing();

		lastFrameStartTime = currentTime;
		Millisecond frameTime = std::chrono::duration_cast<Millisecond>(Clock::now() - currentTime);
	}

	return 0;
}