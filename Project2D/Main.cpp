#include <ResourceManager/ResourceManager.h>
#include <UserInterfaces/UserInterfaces.h>
#include <Graphics/D3D11/D3D11.h>
#include <Graphics/D3D11/D3D11Renderer.h>

int main() {
	ResourceManager& resourceManager = ResourceManager::get();
	resourceManager.init();

	UserInterfaces& userInterfaces = UserInterfaces::get();
	userInterfaces.init();

	D3D11& d3d11 = D3D11::get();
	d3d11.init();

	D3D11Renderer& d3d11Renderer = D3D11Renderer::get();
	d3d11Renderer.init();

	MSG msg{0};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, userInterfaces.getMainWindow(), 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//Logic
		d3d11Renderer.testDraw();
	}

	return 0;
}