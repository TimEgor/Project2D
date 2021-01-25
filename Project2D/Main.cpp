#include <ResourceManager/ResourceManager.h>
#include <UserInterfaces/UserInterfaces.h>
#include <Graphics/D3D12/D3D12.h>
#include <Graphics/D3D12/D3D12Renderer.h>

int main() {
	ResourceManager& resourceManager = ResourceManager::get();
	resourceManager.init();

	UserInterfaces& userInterfaces = UserInterfaces::get();
	userInterfaces.init();

	D3D12& d3d12 = D3D12::get();
	d3d12.init();

	D3D12Renderer& renderer = D3D12Renderer::get();

	MSG msg{0};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, userInterfaces.getMainWindow(), 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		renderer.testDraw();
	}

	return 0;
}