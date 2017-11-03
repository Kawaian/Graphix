#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "../include/Graphics/Shader.h"
#include "../include/Graphics/Transform.h"

namespace gpx
{

	class Camera
	{
	public:
		Camera();
		Camera(glm::vec3 pos, float fieldOfView, float aspect, float nearPlane = .00001f, float farPlane = 1000.0f, float horizontleAngle = .0f, float verticalAngle = .0f);

		const glm::vec3& GetPosition() const { return m_position; };
		void SetPosition(const glm::vec3 pos) { m_position = pos; };
		void OffSetPosition(const glm::vec3& offset) { m_position += offset; };

		float GetFieldOfView() const { return m_fov; };
		void SetFieldOfView(float fov);
		float GetNearPlane() const { return m_near; };
		float GetFarPlane() const { return m_far; };
		void SetNearPlane(float near);
		void SetFarPlane(float far);
		float GetViewPortAscpectRatio() const { return m_viewPort; };
		void  SetViewPortAscpectRatio(float aspect);

		void SetUniforms(const ShaderSmtPtr& shader, Transform transform) const;
		void SetUniforms(const ShaderProgram* shader, Transform transform) const;

		glm::mat4 GetOrientation() const;
		void OffSetOrientation(float up, float right);
		void LookAt(glm::vec3 pos);

		glm::vec3 GetForward() const;///returning unit vectors
		glm::vec3 GetRight() const;
		glm::vec3 GetUp() const;

		glm::mat4 Combined() const;
		glm::mat4 Projection() const;
		glm::mat4 View() const;
	private:
		void Normalize();///Prevent Gimble lock
		glm::vec3 m_position;
		float m_horizAngle;
		float m_vertAngle;
		float m_fov;
		float m_near;
		float m_far;
		float m_viewPort;
	};

}














