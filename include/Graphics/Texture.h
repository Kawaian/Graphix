#pragma once
#include <GL/glew.h>
#include <string>
#include <map>
#include <memory>
#include "../include/Graphics/RefCounter.h"
#include "../include/Core/Types.h"

namespace gpx
{

		enum class TextureFilter : s32
		{
			Linear = GL_LINEAR,
			Nearest = GL_NEAREST
		};

		enum class TextureWrap : s32
		{
			Clamp = GL_CLAMP,
			Repeat = GL_REPEAT,
			ClampToEdge = GL_CLAMP_TO_EDGE,
			ClampToBorder = GL_CLAMP_TO_BORDER,
			MirroredRepeat = GL_MIRRORED_REPEAT
		};

		enum class TextureType
		{
			Texture1D = GL_TEXTURE_1D,
			Texture2D = GL_TEXTURE_2D,
			Texture3D = GL_TEXTURE_3D
		};

		class CTexture;
		class TextureData : public ReferenceCounter
		{
			friend class CTexture;
		public:
			TextureData(TextureType textureType, int width, int height,
				int numTextures, unsigned char** data, GLfloat* filter,
				GLfloat* pattern, GLenum* internalFormat, GLenum* format, GLenum* attachments);
			virtual ~TextureData();

			void Bind(unsigned texSlot);
			void BindAsRenderTarget();

			GLuint GetID() const { return *m_texID; }

			TextureData(const TextureData& other) = delete;
			TextureData& operator=(const TextureData& texture) = delete;
		private:

			void InitTextures(unsigned char** data, GLfloat* filter, GLfloat* pattern, GLenum* internalFormat, GLenum* format);
			void InitRenderTargets(GLenum* attachments);

			TextureType m_textureType;
			GLenum m_textureGLType;
			GLuint* m_texID;
			GLuint m_FBO;
			GLuint m_RBO;
			unsigned m_numTextures;
			int m_width;
			int m_height;
		};

		class CTexture
		{
		public:
			CTexture(const char* texturePath, TextureType textureType = TextureType::Texture2D, GLfloat filter = GL_LINEAR, GLfloat pattern = GL_REPEAT, 
									GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, GLenum attachment = GL_NONE);
			CTexture(int width = 0, int height = 0, unsigned char* data = 0, TextureType textureType = TextureType::Texture2D, GLfloat filter = GL_LINEAR,
									GLfloat pattern = GL_REPEAT, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, GLenum attachment = GL_NONE);
			virtual ~CTexture();

			void Bind(unsigned int unit = 0) const;
			void BindAsRenderTarget();

			unsigned GetWidth() const { return m_texData->m_width; }
			unsigned GetHeight() const { return m_texData->m_height; }

			GLuint GetID() const { return m_texData->GetID(); }

			bool HasDisposed();
			void Dispose();
			CTexture(const CTexture& other) = delete;
			CTexture& operator=(const CTexture& texture) = delete;
		private:
			std::string m_fileName;
			TextureData* m_texData;
			bool m_hasDisposed = false;
		};

	typedef std::shared_ptr<CTexture> Texture;

	Texture CreateTexture(const char* texturePath, TextureType textureType = TextureType::Texture2D,
		GLfloat filter = GL_LINEAR, GLfloat pattern = GL_REPEAT, GLenum attachment = GL_NONE);

	Texture CreateTexture(int width = 0, int height = 0, unsigned char* data = 0,
		TextureType textureType = TextureType::Texture2D, GLfloat filter = GL_LINEAR,
		GLfloat pattern = GL_REPEAT, GLenum attachment = GL_NONE);

}















