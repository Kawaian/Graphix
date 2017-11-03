#include <glm/glm.hpp>
#include "../include/Graphics/Billboard.h"
#include "../include/Graphics/LogManager.h"

static const unsigned int NUM_ROWS = 10;
static const unsigned int NUM_COLS = 10;

using namespace gpx;

Billboard::Billboard(const CTexture* texture)
	:	m_VBO(0)
{
	LOG_INFO("Billboard", "Created VBO : ID is", m_VBO);
	m_texture = texture;

	glm::vec3 positions[NUM_ROWS * NUM_COLS];

	for (unsigned int i = 0; i < NUM_ROWS; ++i)
		for (unsigned int j = 0; j < NUM_COLS; ++j)
			positions[i * NUM_COLS + j] = glm::vec3((float)j, 0.0f, (float)i);

	glGenBuffers(1, &m_VAO);
	glBindBuffer(GL_VERTEX_ARRAY, m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_VERTEX_ARRAY, 0);
}

Billboard::~Billboard(){
	if(m_VBO != 0){
		glDeleteBuffers(1, &m_VBO);
		LOG_INFO("Billboard", "Deleted VBO : ID was", m_VBO);
	}
	if (m_VAO != 0) {
		glDeleteBuffers(1, &m_VAO);
		LOG_INFO("Billboard", "Deleted VAO : ID was", m_VAO);
	}
}

void Billboard::Render(){
	m_texture->Bind(0);
	
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, NUM_ROWS * NUM_COLS);
	
	glDisableVertexAttribArray(0);
}















