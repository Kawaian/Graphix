#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "Shader.h"

namespace gpx
{

	class Text2D
	{
	public:
		Text2D(Texture fontMap);
		~Text2D();

		void Render(const char* text, int x, int y, int s);

		Text2D& operator=(const Text2D& other) = delete;
		Text2D(const Text2D& other) = delete;
	private:
		ShaderSmtPtr m_shader;
		Texture m_fontMap;
		GLuint m_VAO;
		GLuint m_VBO_Vertex;
		GLuint m_VBO_UV;
	};
}