#pragma once

#include <EntityManager/EntityComponent.h>

enum CanvasRenderingMode {
	OverlayCanvasRenderingMode,
	WorldCanvasRenderingMode
};

class CanvasEntityComponent final : public EntityComponent {
private:
	CanvasRenderingMode mode = OverlayCanvasRenderingMode;

public:
	CanvasEntityComponent() = default;
	CanvasEntityComponent(CanvasRenderingMode mode) : mode(mode) {}

	CanvasRenderingMode getRenderingMode() const { return mode; }
	void setRenderingMode(CanvasRenderingMode newMode) { mode = newMode; }

	virtual EntityComponentType getEntityComponentType() const override { return CanvasEntityComponentType; };
};