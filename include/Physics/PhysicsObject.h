#pragma once

#include <glm/glm.hpp>
#include "BoundingSphere.h"
#include <iostream>

namespace gpx
{

	class PhysicsObject {
	public:
		PhysicsObject(const glm::vec3& position, const glm::vec3 velocity = glm::vec3(0.0, 0.0, 0.0), float radius = 1.0)
			:
			m_position(position),
			m_tempPosition(position),
			m_velocity(velocity),
			m_radius(radius),
			m_boundingSphere(position, radius)
		{}

		void Integrate(float delta);

		glm::vec3 GetPosition() const{ return m_position; }
		glm::vec3 GetVelocity() const{ return m_velocity; }
		void SetVelocity(const glm::vec3& velocity) { m_velocity = velocity; }
		void SetPosition(const glm::vec3& position) { m_position = position; }
		float GetRadius() { return m_radius; }
		void SetRadius(float radius) { m_radius = radius; }

		const Collider& GetBoundingSphere(){
			glm::vec3 distance = m_position - m_tempPosition;
			m_tempPosition = m_position;
			m_boundingSphere.Transform(distance);
			return m_boundingSphere;
		}

	private:
		glm::vec3 m_position;
		glm::vec3 m_tempPosition;
		glm::vec3 m_velocity;

		//TODO THIS IS TEMP!
		float m_radius;
		mutable BoundingSphere m_boundingSphere;
	};
}