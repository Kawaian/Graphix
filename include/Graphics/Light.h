#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include "Texture.h"
#include "Shader.h"

namespace gpx
{


	struct Attenuation
	{
		float constant = 1.0f;
		float linear = 0.09f;
		float exponential = 0.132f;
	};

	class BaseLight
	{
	public:
		BaseLight()
			:
			m_intensities(glm::vec3(1.0, 1.0, 1.0)),
			m_ambientCoeff(0.065f)
		{}
		virtual ~BaseLight(){}

		virtual void SetUniforms(const ShaderSmtPtr& shader) const = 0;

		void SetIntensity(const glm::vec3& intensity){
			m_intensities = intensity;
		}
		glm::vec3 GetIntensity() const{
			return m_intensities;
		}
		void SetAmbientCoefficient(float ambientCoefficient){
			m_ambientCoeff = ambientCoefficient;
		}
		float GetAmbientCoefficient() const{
			return m_ambientCoeff;
		}
	protected:
		glm::vec3 m_intensities;//a.k.a color of light
		float m_ambientCoeff;
	};

	class DirectionalLight : public BaseLight
	{
	public:
		DirectionalLight()
			:
			m_direction(0.0, 0.0, 0.0)
		{}
		~DirectionalLight(){}

		void SetUniforms(const ShaderSmtPtr& shader) const
		{
			shader->UpdateUniform3fv("directionalLight.base.intensities", glm::value_ptr(BaseLight::m_intensities));
			shader->UpdateUniform1f("directionalLight.base.ambientCoeff", BaseLight::m_ambientCoeff);
			shader->UpdateUniform3fv("directionalLight.direction", glm::value_ptr(this->m_direction));
		}

		void SetDirection(const glm::vec3& direction)
		{
			m_direction = direction;
		}
		glm::vec3 GetDirection()
		{
			return m_direction;
		}
	protected:
		glm::vec3 m_direction;
	};

	class PointLight : public BaseLight
	{
	public:
		PointLight()
			:
			m_position(glm::vec3(0.0, 0.0, 0.0)),
			m_attenuation(Attenuation())
		{}

		~PointLight(){}

		virtual void SetUniforms(const ShaderSmtPtr& shader) const{
			shader->UpdateUniform3fv("pointLight.position", glm::value_ptr(this->m_position));
			shader->UpdateUniform3fv("pointLight.base.intensities", glm::value_ptr(this->m_intensities));
			shader->UpdateUniform1f("pointLight.attenuation.constant", this->m_attenuation.constant);
			shader->UpdateUniform1f("pointLight.attenuation.linear", this->m_attenuation.linear);
			shader->UpdateUniform1f("pointLight.attenuation.exponential", this->m_attenuation.exponential);
			shader->UpdateUniform1f("pointLight.base.ambientCoeff", this->m_ambientCoeff);

				float lightMax = std::fmaxf(std::fmax(GetIntensity().r, GetIntensity().g),
				GetIntensity().b);

			float radius = 	(-m_attenuation.linear +
			/*	std::sqrtf(std::pow(m_attenuation.linear, 2.0f) - 4.0f * m_attenuation.exponential *
					(m_attenuation.constant - (256.0f / 5.0f) * lightMax))) / (2.0f * m_attenuation.exponential);
			*/
			std::sqrt(std::pow(m_attenuation.linear, 2.0f) - 4.0f * m_attenuation.exponential *
							(m_attenuation.constant - (256.0f / 5.0f) * lightMax))) / (2.0f * m_attenuation.exponential);


			shader->UpdateUniform1f("pointLight.radius", radius);
		}
		
		virtual void SetUniforms(const ShaderProgram* shader) const {
			shader->UpdateUniform3fv("pointLight.position", glm::value_ptr(this->m_position));
			shader->UpdateUniform3fv("pointLight.base.intensities", glm::value_ptr(this->m_intensities));
			shader->UpdateUniform1f("pointLight.attenuation.constant", this->m_attenuation.constant);
			shader->UpdateUniform1f("pointLight.attenuation.linear", this->m_attenuation.linear);
			shader->UpdateUniform1f("pointLight.attenuation.exponential", this->m_attenuation.exponential);
			shader->UpdateUniform1f("pointLight.base.ambientCoeff", this->m_ambientCoeff);
			float lightMax = std::fmaxf(std::fmax(GetIntensity().r, GetIntensity().g),
				GetIntensity().b);

			float radius = 	(-m_attenuation.linear +
			/*	std::sqrtf(std::pow(m_attenuation.linear, 2.0f) - 4.0f * m_attenuation.exponential *
					(m_attenuation.constant - (256.0f / 5.0f) * lightMax))) / (2.0f * m_attenuation.exponential);
		*/

			std::sqrt(std::pow(m_attenuation.linear, 2.0f) - 4.0f * m_attenuation.exponential *
					(m_attenuation.constant - (256.0f / 5.0f) * lightMax))) / (2.0f * m_attenuation.exponential);


			shader->UpdateUniform1f("pointLight.radius", radius);
		}

		float GetInfluenceRadius()
		{
			float lightMax = std::fmaxf(std::fmax(GetIntensity().r, GetIntensity().g),
				GetIntensity().b);

			return (-m_attenuation.linear +
		/*		std::sqrtf(std::pow(m_attenuation.linear, 2.0f) - 4.0f * m_attenuation.exponential *
					(m_attenuation.constant - (256.0f / 5.0f) * lightMax))) / (2.0f * m_attenuation.exponential);
		*/
			std::sqrt(std::pow(m_attenuation.linear, 2.0f) - 4.0f * m_attenuation.exponential *
					(m_attenuation.constant - (256.0f / 5.0f) * lightMax))) / (2.0f * m_attenuation.exponential);
		}

		void SetPosition(const glm::vec3& position){
			m_position = position;
		}
		void SetAttenuation(Attenuation attenuation){
			m_attenuation = attenuation;
		}
		glm::vec3 GetPosition() const{
			return m_position;
		}
		Attenuation GetAttenuation() const{
			return m_attenuation;
		}
	protected:
		glm::vec3 m_position;
		Attenuation m_attenuation;
	};
}
