#include "D3D11SpriteBatchManager.h"

#include <Graphics/RenderingOrder.h>

#include <cassert>

D3D11SpriteBatchManager::D3D11SpriteBatchManager() : preparingBatchesCount(0) {}

//void D3D11SpriteBatchManager::prepareRenderingData(const std::vector<RenderingOrderNode*>& nodes, RenderingOrderType nodesType, size_t vertecesCount, size_t indecesCount) {
//	if (nodes.empty()) {
//		return;
//	}
//
//	size_t nodesCount = nodes.size();
//
//	size_t currentNodeIndex = 0;
//	size_t currentBatchIndex = 0;
//	size_t currentNodesCount = nodesCount;
//
//	while (currentNodeIndex < nodesCount) {
//		if (currentBatchIndex >= batches.size()) {
//			batches.emplace_back();
//		}
//
//		D3D11SpriteBatch& batch = batches[currentBatchIndex];
//
//		if (!batch.isInit()) {
//			batch.init(BATCH_SIZE);
//		}
//
//		currentNodeIndex += batch.buildData(nodes, currentNodeIndex, nodesType);
//		++currentBatchIndex;
//	}
//
//	preparingBatchesCount = batches.size();
//}

void D3D11SpriteBatchManager::releaseUselessBranches() {
	size_t currentBatchesCount = batches.size();
	size_t uselessBatchesCount = currentBatchesCount - preparingBatchesCount;

	for (size_t i = 0; i < uselessBatchesCount; ++i) {
		D3D11SpriteBatch& uselessBatch = batches[currentBatchesCount - i - 1];

		if (uselessBatch.isInit()) {
			uselessBatch.release();
		}

		batches.pop_back();
	}
}

void D3D11SpriteBatchManager::release() {
	batches = std::vector<D3D11SpriteBatch>();
	preparingBatchesCount = 0;
}
