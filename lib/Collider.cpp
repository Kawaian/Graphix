#include "../include/Physics/Collider.h"

#include <cstdlib>
#include <iostream>

#include "../include/Physics/BoundingSphere.h"

using namespace gpx;

Collider::~Collider(){}

IntersectData Collider::Intersect(const Collider& other) const{
	if(m_type == SPHERE_COLLISION && other.GetType() == SPHERE_COLLISION)
	{
		BoundingSphere* self = (BoundingSphere*)this;
		return self->IntersectsSphere((BoundingSphere&)other);
	}
	std::cout << "ERROR : NON SPECIFIED COLLIDERS !\n";
	exit(1);
	return IntersectData(false, 0.0);
}

void Collider::Transform(const glm::vec3& translate){}
