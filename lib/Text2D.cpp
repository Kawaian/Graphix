#include "../include/Graphics/Text2D.h"
#include <vector>

using namespace gpx;

Text2D::Text2D(Texture fontMap)
	:	
		m_fontMap(fontMap),
		m_shader(CreateShader("res/shaders/text2D.glslv", "res/shaders/text2D.glslf"))
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO_Vertex);
	glGenBuffers(1, &m_VBO_UV);
}


void Text2D::Render(const char* text, int x, int y, int size)
{
	unsigned length = strlen(text);

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;

	for (unsigned i = 0; i < length; i++)
	{
		glm::vec2 vertex_upl = glm::vec2  (x + i * size, y + size       );
		glm::vec2 vertex_upr = glm::vec2  (x + i * size + size, y + size);
		glm::vec2 vertex_downr = glm::vec2(x + i * size + size, y       );
		glm::vec2 vertex_downl = glm::vec2(x + i * size, y              );
		
		vertices.push_back(vertex_upl);
		vertices.push_back(vertex_downl);
		vertices.push_back(vertex_upr);

		vertices.push_back(vertex_downr);
		vertices.push_back(vertex_upr);
		vertices.push_back(vertex_downl);

		char charac = text[i];
		float UV_x = (charac % 16) / 16.0f;
		float UV_y = (charac / 16) / 16.0f;

		glm::vec2 UV_upl = glm::vec2(UV_x, UV_y);
		glm::vec2 UV_upr = glm::vec2(UV_x + 1.0f / 16.0f, UV_y);
		glm::vec2 UV_downr = glm::vec2(UV_x + 1.0f / 16.0f, (UV_y + 1.0f / 16.0f));
		glm::vec2 UV_downl = glm::vec2(UV_x, (UV_y + 1.0f / 16.0f));
	
		UVs.push_back(UV_upl);
		UVs.push_back(UV_downl);
		UVs.push_back(UV_upr);

		UVs.push_back(UV_downr);
		UVs.push_back(UV_upr);
		UVs.push_back(UV_downl);
	}

	glBindVertexArray(m_VAO);


	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Vertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_UV);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	m_shader->Bind();
	m_fontMap->Bind(0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Vertex);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_UV);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindVertexArray(0);
}

Text2D::~Text2D()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO_Vertex);
	glDeleteBuffers(1, &m_VBO_UV);
}
