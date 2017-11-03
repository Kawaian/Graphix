#include "../include/Physics/AABB.h"
#include <glm/gtx/component_wise.hpp>

using namespace gpx;

IntersectData AABB::IntersectsAABB(const AABB& other) const{
	glm::vec3 distances1 = other.m_minExtents - m_maxExtents;
	glm::vec3 distances2 = m_minExtents - other.m_maxExtents;
	glm::vec3 distances = glm::max(distances1, distances2);

	float maxDistance = distances[0];

	for(unsigned i = 0; i < 2; ++i){
		if(distances[i] > maxDistance)
			maxDistance = distances[i];
	}
    
	return IntersectData(maxDistance < 0, maxDistance);
}
