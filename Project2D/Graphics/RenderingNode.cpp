#include "RenderingNode.h"

RenderingNode::RenderingNode(TransformMatrix* transform, ResourceReference material)
	: transform(transform), materialResource(material) {}

RenderingNode::~RenderingNode() {}

SpriteRenderingNode::SpriteRenderingNode(TransformMatrix* transform, ResourceReference material, ResourceReference sprite)
	: RenderingNode(transform, material), spriteResource(sprite) {}

bool SpriteRenderingNode::sortPredicate(const RenderingNode* node1, const RenderingNode* node2) {
	SpriteRenderingNode* spriteNode1 = (SpriteRenderingNode*)(node1);
	SpriteRenderingNode* spriteNode2 = (SpriteRenderingNode*)(node2);

	if (spriteNode1->materialResource.getResourceID() == spriteNode2->materialResource.getResourceID()) {
		return spriteNode1->spriteResource.getResourceID() < spriteNode2->spriteResource.getResourceID();
	}
	else {
		return spriteNode1->materialResource.getResourceID() < spriteNode2->materialResource.getResourceID();
	}
}
