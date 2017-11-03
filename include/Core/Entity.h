#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../include/Graphics/Transform.h"

namespace gpx
{

	class Entity {
	public:
		Entity	(
					const glm::vec3& position = glm::vec3(0.0, 0.0, 0.0),
					const glm::vec3& rotation = glm::vec3(0.0, 0.0, 0.0),
					const glm::vec3& scale = glm::vec3(1.0, 1.0, 1.0)
				)
		:	
				m_transform(position, rotation, scale)
		{}
		Entity	(
					const Transform& transform
				)
			:
				m_transform(transform)
		{}

		virtual ~Entity(){}

		void SetTransform(const Transform& transform) { m_transform = transform; }
		Transform GetTransform() const { return m_transform; }

		void SetPosition(const glm::vec3& position) { m_transform.SetPos(position); }
		void SetRotation(const glm::vec3& rotation) { m_transform.SetRot(rotation); }
		void SetScale(const glm::vec3& scale) { m_transform.SetScale(scale); }
		glm::vec3 GetPosition() const { return m_transform.GetPos(); }
		glm::vec3 GetRotation() const { return m_transform.GetRot(); }
		glm::vec3 GetScale() const { return m_transform.GetScale(); }
	protected:
		Transform m_transform;
	};
}
