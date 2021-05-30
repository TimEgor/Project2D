#include <Core.h>

#include <LevelManager/LevelManager.h>
#include <EntityManager/Entity.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <EntityManager/EntityComponents/CppGameLogicEntityComponent.h>
#include <Graphics/Node.h>
#include <ResourceManager/ResourceManager.h>
#include <GameLogic/CPP/CppGameLogicClassManager.h>

#include <Windows.h>

class TestClass final : public CppGameLogicClass {
public:
	TestClass() = default;

	virtual void update(float deltaTime) override {
		Node* node = getComponent()->getParent()->getNode();
		node->setRotation(node->getRotation() + 10.0f * deltaTime);
	}
};

CPPGameLogicClassRegistry("TestClass", TestClass);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int show) {
	Core& core = Core::get();

	core.init();

	Level* level = LevelManager::get().createLevel();

	Entity* entity = level->createSceneEntity();
	SpriteRendererEntityComponent* spriteComponent = level->createEntityComponent<SpriteRendererEntityComponent>(entity, ResourceManager::get().getResourceFromArchive("testTexture.png"));

	SceneNode* node = (SceneNode*)(entity->getNode());
	node->setPositionX(2.0f);

	TestClass* testLogicClass = (TestClass*)(level->createEntityComponent<CppGameLogicEntityComponent>(entity, "TestClass"));

	core.run();
	core.release();
}