#include "ResourceReference.h"

ResourceReference& ResourceReference::operator=(const ResourceReference& reference) {
	ReferenceBaseType::operator=(reference);
	return *this;
}
