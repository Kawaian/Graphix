#pragma once
#include <glm/glm.hpp>

#include "IntersectData.h"
#include "Collider.h"

namespace gpx
{
	class BoundingSphere : public Collider
	{
	public:

		BoundingSphere(const glm::vec3& center, float radius)
			:
			Collider(Collider::SPHERE_COLLISION),
			m_center(center),
			m_radius(radius)
		{}

		virtual ~BoundingSphere(){}

		IntersectData IntersectsSphere(const BoundingSphere& other) const;

		virtual void Transform(const glm::vec3& translation) override;

		virtual glm::vec3 GetCenter() const { return m_center; }

		float GetRadius() const{ return m_radius; }
	private:
		glm::vec3 m_center;
		float m_radius;
	};
}
