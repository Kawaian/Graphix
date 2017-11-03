#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "../core/ResourceManager.h"

namespace gpx
{
	class SpriteBatch
	{
	public:
		SpriteBatch(const ShaderProgram* shader);
		~SpriteBatch();

		void RenderSprite(const CTexture* texture, const glm::vec2& position, const glm::vec2& size = glm::vec2(10.0f, 10.0f),
			GLfloat rotate = 0.0f, const glm::vec3& color = glm::vec3(1.0f));

		inline void RenderSprite(GLuint texId, const glm::vec2& position, const glm::vec2& size = glm::vec2(10.0f, 10.0f),
			GLfloat rotate = 0.0f, const glm::vec3& color = glm::vec3(1.0f))
		{
			m_shader->Bind();
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(position, 0.0f));

			model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
			model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5f * size.y, 0.0f));

			model = glm::scale(model, glm::vec3(size, 1.0f));

			m_shader->UpdateUniformMatrix4f("model", model);
			m_shader->UpdateUniform3fv("spriteColor", glm::value_ptr(color));
			
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, texId);

			glBindVertexArray(m_quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		
		SpriteBatch& operator=(const SpriteBatch& other) = delete;
		SpriteBatch(const SpriteBatch& other) = delete;
	private:
		const ShaderProgram* m_shader;
		GLuint m_quadVAO;
		void INIT();
	};
}