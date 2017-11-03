#include "../include/Physics/BoundingSphere.h"
#include "../include/Physics/IntersectData.h"

using namespace gpx;

IntersectData BoundingSphere::IntersectsSphere(const BoundingSphere& other) const{
	float rDistance = m_radius + other.m_radius;
	float cDistance = glm::length(other.m_center - m_center);
	float distance = cDistance - rDistance;
	return IntersectData(cDistance < rDistance, distance);
}

void BoundingSphere::Transform(const glm::vec3& translation){
	m_center += translation;
}
