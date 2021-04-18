#pragma once

#include <Graphics/D3D11/D3D11.h>
#include <EntityManager/EntityComponents/LabelComponent.h>

class D3D11LabelComponent final : public LabelComponent {
private:
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	size_t indexCount = 0;

	size_t allocatedCharSize = 0;

public:
	D3D11LabelComponent() : LabelComponent() {}
	D3D11LabelComponent(Heap* heap) : LabelComponent(heap) {}
	D3D11LabelComponent(const char* text) : LabelComponent(text) {}
	D3D11LabelComponent(const char* text, Heap* heap) : LabelComponent(text, heap) {}
	~D3D11LabelComponent() { release(); }

	ID3D11Buffer* getVertexBuffer() const { return vertexBuffer; }
	ID3D11Buffer* getIndexBuffer() const { return indexBuffer; }
	size_t getIndexCount() const { return indexCount; }

	virtual void release();
	virtual void updateBuffers() override;
};