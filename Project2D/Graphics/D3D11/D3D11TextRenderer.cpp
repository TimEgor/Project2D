#include "D3D11TextRenderer.h"

D3D11TextRenderer& D3D11TextRenderer::get() {
    static D3D11TextRenderer uniqueD3D11TextRenderer;
    return uniqueD3D11TextRenderer;
}

bool D3D11TextRenderer::init() {
    return true;
}

void D3D11TextRenderer::release() {
}

bool D3D11TextRenderer::addFont(const ResourceName& fontName) {
    return false;
}

void D3D11TextRenderer::removeFont(const ResourceName& fontName) {
}

void D3D11TextRenderer::drawText(size_t strLen, const char* str) {

}
