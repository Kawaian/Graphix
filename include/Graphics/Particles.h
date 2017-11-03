//#pragma once
//
//#include <glm/glm.hpp>
//#include "../core/ShaderTechnique.h"
//
//namespace Spiky
//{
//
//
//	struct Particle
//	{
//		float type;
//		glm::vec3 position;
//		glm::vec3 velocity;
//		float lifeTime;
//	};
//
//	//TODO Finish Implementing this class After Billboard is done !
//	class ParticleSystem
//	{
//	public:
//		ParticleSystem(const glm::vec3& pos);
//		~ParticleSystem();
//
//		void Render(){}
//
//		ParticleSystem& operator=(const ParticleSystem& other) = delete;
//		ParticleSystem(const ParticleSystem& other) = delete;
//	private:
//		bool m_isFirst;
//		unsigned m_currVBO;
//		unsigned m_currTFBO;
//		GLuint m_VBO[2];
//		GLuint m_TFBO[2];
//		PSUpdateTechnique m_technique;
//	};
//}