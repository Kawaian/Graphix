#include <glm/gtc/matrix_transform.hpp>
#include "../include/Graphics/SpriteBatch.h"

using namespace gpx;

SpriteBatch::SpriteBatch(const ShaderProgram* shader)
	:
		m_shader(shader)
{ INIT(); }

SpriteBatch::~SpriteBatch()
{
	glDeleteVertexArrays(1, &m_quadVAO);
}

void SpriteBatch::INIT()
{
	GLuint VBO;
	GLfloat  vertices[]
	{
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(m_quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteBatch::RenderSprite(const CTexture* texture, const glm::vec2& position, const glm::vec2& size,
	GLfloat rotate, const glm::vec3& color)
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

	texture->Bind(0);

	glBindVertexArray(m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}