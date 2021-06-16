#pragma once

#include <ResourceManager/Resource.h>

enum MaterialSortingModeType {
	ForwardMaterialSortingModeType,
	ReverseMaterialSortingModeType
};

class MaterialResource : public Resource {
protected:
	MaterialSortingModeType sortingMode = ForwardMaterialSortingModeType;

public:
	MaterialResource() = default;

	MaterialSortingModeType getSortingMode() const { return sortingMode; }
};