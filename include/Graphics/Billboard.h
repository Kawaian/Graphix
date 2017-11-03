#pragma once

#include <GL/glew.h>

#include "../include/Graphics/Texture.h"

namespace gpx
{

	class Billboard {
	public:
		Billboard(const CTexture* texture);
		~Billboard();

		void Render();
		void SetTexture(const CTexture* texture){ m_texture = texture; }

	private:
		GLuint m_VAO;
		GLuint m_VBO;
		const CTexture* m_texture;
	};
}