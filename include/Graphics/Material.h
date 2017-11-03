#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

#include "Texture.h"
#include "Shader.h"

namespace gpx
{

	struct TextureMap
	{
	public:
		TextureMap(Texture diffuse, Texture normal = CreateTexture("assets/images/DefaultNormalMap.jpg"),
			Texture disp = CreateTexture("assets/images/DefaultDispMap.png"), float dispScale = 0.05f,
			float dispBias = -0.3f)
			:
			diffuseMap(diffuse),
			normalMap(normal),
			dispMap(disp),
			dispMapScale(dispScale),
			dispMapBias(dispBias)
		{}

		TextureMap(const char* diffuse = "assets/images/Debug.png", const char* normal = "assets/images/DefaultNormalMap.jpg",
			const char* disp = "assets/images/DefaultDispMap.png", float dispScale = 0.0f,
			float dispBias = 0.0f)
			:
			diffuseMap(CreateTexture(diffuse)),
			normalMap(CreateTexture(normal)),
			dispMap(CreateTexture(disp)),
			dispMapScale(dispScale),
			dispMapBias(dispBias)
		{}

		Texture diffuseMap;
		Texture normalMap;
		Texture dispMap;
		float dispMapScale;
		float dispMapBias;
	};
	class Material{
	public:
		Material(TextureMap textureMap = TextureMap(), float shininess = 8.0f,
			const glm::vec3& specularColor = glm::vec3(1.0, 1.0, 1.0))
			:
			m_textureMap(textureMap),
			m_shininess(shininess),
			m_specularColor(specularColor)
		{}
		void Bind(const ShaderSmtPtr& shader){
			m_textureMap.diffuseMap->Bind(0);
			m_textureMap.normalMap->Bind(1);
			m_textureMap.dispMap->Bind(2);
			glUniform1i(shader->GetUniformLocation("materialTex"), 0);
			glUniform1i(shader->GetUniformLocation("normalTex"), 1);
			glUniform1i(shader->GetUniformLocation("dispMap"), 2);
		}

		void Bind(const ShaderProgram* shader) {
			m_textureMap.diffuseMap->Bind(0);
			m_textureMap.normalMap->Bind(1);
			m_textureMap.dispMap->Bind(2);
			glUniform1i(shader->GetUniformLocation("diffuseMap"), 0);
			glUniform1i(shader->GetUniformLocation("normalMap"), 1);
			glUniform1i(shader->GetUniformLocation("dispMap"), 2);
		}


		void SetUniforms(const ShaderSmtPtr& shader) const{
			shader->UpdateUniform3fv("material.specularColor", glm::value_ptr(this->m_specularColor));
			shader->UpdateUniform1f("material.shininess", this->m_shininess);

			float baseBias = m_textureMap.dispMapScale / 2.0f;
			shader->UpdateUniform1f("dispMapScale", m_textureMap.dispMapScale);
			shader->UpdateUniform1f("dispMapBias", -baseBias + baseBias * m_textureMap.dispMapBias);
		}

		void SetUniforms(const ShaderProgram* shader) const {
			shader->UpdateUniform3fv("material.specularColor", glm::value_ptr(this->m_specularColor));
			shader->UpdateUniform1f("material.shininess", this->m_shininess);

			float baseBias = m_textureMap.dispMapScale / 2.0f;
			shader->UpdateUniform1f("dispMapScale", m_textureMap.dispMapScale);
			shader->UpdateUniform1f("dispMapBias", -baseBias + baseBias * m_textureMap.dispMapBias);
		}

		void SetShininess(float shininess){
			m_shininess = shininess;
		}
		void SetSpecularColor(glm::vec3 specularColor){
			m_specularColor = specularColor;
		}
		float GetShininess(){
			return m_shininess;
		}
		glm::vec3 GetSpecularColor(){
			return m_specularColor;
		}

		void SetTextureMap(const TextureMap& textureMap){
			m_textureMap = textureMap;
		}

		TextureMap GetTextureMap(){
			return m_textureMap;
		}

	protected:
		TextureMap m_textureMap;
		float m_shininess;
		glm::vec3 m_specularColor;
	};
}