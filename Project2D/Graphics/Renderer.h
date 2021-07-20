#pragma once

class Renderer {
public:
	virtual ~Renderer() {}

	virtual void beginDrawing() = 0;
	virtual void endRendering() = 0;

	virtual void testRender() = 0;
};