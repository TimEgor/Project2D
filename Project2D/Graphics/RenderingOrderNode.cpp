#include "RenderingOrderNode.h"

RenderingOrderNode::RenderingOrderNode(TransformMatrix* transform, ResourceReference material)
	: transform(transform), materialResource(material) {}

RenderingOrderNode::~RenderingOrderNode() {}

SpriteRenderingOrderNode::SpriteRenderingOrderNode(TransformMatrix* transform, ResourceReference material, ResourceReference sprite)
	: RenderingOrderNode(transform, material), spriteResource(sprite) {}

bool SpriteRenderingOrderNode::sortPredicate(const RenderingOrderNode* node1, const RenderingOrderNode* node2) {
	SpriteRenderingOrderNode* spriteNode1 = (SpriteRenderingOrderNode*)(node1);
	SpriteRenderingOrderNode* spriteNode2 = (SpriteRenderingOrderNode*)(node2);

	if (spriteNode1->materialResource.getResourceID() == spriteNode2->materialResource.getResourceID()) {
		return spriteNode1->spriteResource.getResourceID() < spriteNode2->spriteResource.getResourceID();
	}
	else {
		return spriteNode1->materialResource.getResourceID() < spriteNode2->materialResource.getResourceID();
	}
}

bool TextRenderingOrderNode::sortPredicate(const RenderingOrderNode* node1, const RenderingOrderNode* node2) {
	return SpriteRenderingOrderNode::sortPredicate(node1, node2);
}
