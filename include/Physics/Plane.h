#pragma once

#include <glm/glm.hpp>
#include "../physics/BoundingSphere.h"

namespace gpx
{
	class Plane
	{
	public:
		Plane(const glm::vec3& normal, float distance)
			:
			m_normal(normal),
			m_distance(distance)
		{}

		Plane Normalized() const;
		IntersectData IntersectSphere(const BoundingSphere& other) const;

	private:
		const glm::vec3 m_normal;
		const float m_distance;
	};
}