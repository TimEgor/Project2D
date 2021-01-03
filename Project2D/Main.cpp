#include <ResourceManager/ResourceManager.h>

int main() {
	ResourceManager& resourceManager = ResourceManager::get();
	resourceManager.init();

	ResourceReference testFile =  resourceManager.getResourceFromArchive("Test");
	char* fileData = (char*)(testFile.getResource<DefaultResource>().getData());

	return 0;
}