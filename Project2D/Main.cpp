#include <ResourceManager/ResourceManager.h>
#include <UserInterfaces/UserInterfaces.h>
#include <Graphics/D3D12/D3D12.h>

int main() {
	ResourceManager& resourceManager = ResourceManager::get();
	resourceManager.init();

	UserInterfaces& userInterfaces = UserInterfaces::get();
	userInterfaces.init();

	D3D12& d3d12 = D3D12::get();
	d3d12.init();

	return 0;
}