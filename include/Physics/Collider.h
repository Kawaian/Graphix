#pragma once

#include "IntersectData.h"
#include <iostream>
#include <glm/glm.hpp>

namespace gpx
{

	class Collider
	{
	public:
		enum ColliderType
		{
			SPHERE_COLLISION,
			AABB_COLLISION,
			NUM_COLLISION_TYPES
		};

		Collider(ColliderType type)
			:
			m_type(type)
		{}
		virtual ~Collider();

		IntersectData Intersect(const Collider& other) const;
		virtual void Transform(const glm::vec3& translation);

		virtual glm::vec3 GetCenter() const { return glm::vec3(0.0f, 0.0f, 0.0f); }

		ColliderType GetType() const { return m_type; }
	private:
		ColliderType m_type;
	};
}