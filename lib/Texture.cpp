
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "../include/Graphics/Texture.h"
#include "../include/Graphics/libs/stb_image.h"
#include <cassert>
#include <boost/log/trivial.hpp>
#include <sstream>
#include "../include/Core/Exceptions.h"

namespace gpx
{

	Texture CreateTexture(const char* texturePath, TextureType textureType, GLfloat filter, GLfloat pattern, GLenum attachment)
	{
		return std::make_shared<CTexture>(texturePath, textureType, filter, pattern);
	}

	Texture CreateTexture(int width, int height, unsigned char* data, TextureType textureType, GLfloat filter, GLfloat pattern,
		GLenum* internalFormat, GLenum* format, GLenum attachment)
	{
		return std::make_shared<CTexture>(width, height, data, textureType, filter, pattern);
	}

			TextureData::TextureData(TextureType textureType, int width, int height,
								int numTextures, unsigned char** data, GLfloat* filter,
								GLfloat* pattern, GLenum* internalFormat, GLenum* format, GLenum* attachments)
			:
				m_textureType(textureType),
				m_numTextures(numTextures),
				m_FBO(0),
				m_RBO(0),
				m_width(width),
				m_height(height)
		{

			m_texID = new GLuint[numTextures];
			InitTextures(data, filter, pattern, internalFormat, format);
			InitRenderTargets(attachments);
			
			switch (m_textureType)
			{
			case TextureType::Texture1D:
				m_textureGLType = GL_TEXTURE_1D;
				break;
			case TextureType::Texture2D:
				m_textureGLType = GL_TEXTURE_2D;
				break;
			case TextureType::Texture3D:
				m_textureGLType = GL_TEXTURE_3D;
				break;
			}
		}

		TextureData::~TextureData()
		{
			if (*m_texID)
				glDeleteTextures(m_numTextures, m_texID);
			if (m_texID)
				delete[] m_texID;
			if (m_FBO)
				glDeleteFramebuffers(1, &m_FBO);
			if (m_RBO)
				glDeleteRenderbuffers(1, &m_RBO);
		}

		void TextureData::InitTextures(unsigned char** data, GLfloat* filter, GLfloat* pattern,
											GLenum* internalFormat, GLenum* format)
		{
			glGenTextures(m_numTextures, m_texID);
			for (unsigned i = 0; i < m_numTextures; ++i)
			{
					BOOST_LOG_TRIVIAL(trace) << "Building Texture at location h" << std::hex << m_texID << std::dec;
				switch (m_textureType)
				{
				case TextureType::Texture1D:
					glBindTexture(GL_TEXTURE_1D, m_texID[i]);
					glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, filter[i]);
					glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, filter[i]);

					glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, pattern[i]);
					
					glTexImage2D(GL_TEXTURE_1D, 0, internalFormat[i], m_width, m_height, 0, format[i], GL_UNSIGNED_BYTE, data[i]);
					break;
				case TextureType::Texture2D:
					glBindTexture(GL_TEXTURE_2D, m_texID[i]);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter[i]);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter[i]);

					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, pattern[i]);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, pattern[i]);

					glTexImage2D(GL_TEXTURE_2D, 0, internalFormat[i], m_width, m_height, 0, format[i], GL_UNSIGNED_BYTE, data[i]);
				
					if (filter[i] == GL_NEAREST_MIPMAP_NEAREST ||
						filter[i] == GL_NEAREST_MIPMAP_LINEAR ||
						filter[i] == GL_LINEAR_MIPMAP_NEAREST ||
						filter[i] == GL_LINEAR_MIPMAP_LINEAR)
					{


						glGenerateMipmap(GL_TEXTURE_2D);
						GLfloat maxAnisotropy;
						glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
						glTexParameterf(m_textureGLType, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy > 8.0f ? 8.0f : (maxAnisotropy < 0 ? 0.0f : maxAnisotropy));
					}

					break;
				case TextureType::Texture3D:
					glBindTexture(GL_TEXTURE_3D, m_texID[i]);
					glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter[i]);
					glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter[i]);
											   
					glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, pattern[i]);
					glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, pattern[i]);
					glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, pattern[i]);

					glTexImage2D(GL_TEXTURE_3D, 0, internalFormat[i], m_width, m_height, 0, format[i], GL_UNSIGNED_BYTE, data[i]);
					break;
				default:
					break;
				}
			}
		}

		void TextureData::InitRenderTargets(GLenum* attachments)
		{
			if (attachments == 0)
				return;

			GLenum* drawBuffers = new GLenum[m_numTextures];
			bool hasDepth = false;

			for (unsigned i = 0; i < m_numTextures; ++i)
			{
				if (attachments[i] == GL_DEPTH_ATTACHMENT)
				{
					drawBuffers[i] = GL_NONE;
					hasDepth = true;
				}
				else
					drawBuffers[i] = attachments[i];

				if (attachments[i] == GL_NONE)
					continue;

				if (m_FBO == 0)
				{
					glGenFramebuffers(1, &m_FBO);
					glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
				}

				switch (m_textureType)
				{
				case TextureType::Texture1D:
					glFramebufferTexture1D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_1D, m_texID[0], 0);
					break;
				case TextureType::Texture2D:
					glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_2D, m_texID[0], 0);
					break;
				case TextureType::Texture3D:
					glFramebufferTexture3D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_3D, m_texID[0], 0, 0);
					break;
				default:
					break;
				}
			}

			if (m_FBO == 0)
				return;

			if (!hasDepth)
			{
				glGenRenderbuffers(1, &m_RBO);
				glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
			}

			glDrawBuffers(m_numTextures, drawBuffers);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				BOOST_LOG_TRIVIAL(fatal) << "FrameBuffer Creation failed; make sure you've updated your graphics driver recently";
				assert(false);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			delete[] drawBuffers;
		}

		void TextureData::Bind(unsigned texSlot)
		{

			glBindTexture(m_textureGLType, m_texID[texSlot]);
		}

		void TextureData::BindAsRenderTarget(){
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
			glViewport(0, 0, m_width, m_height);
		}

		CTexture::CTexture(const char* fileName, TextureType textureType, GLfloat filter, GLfloat pattern, GLenum internalFormat,
					GLenum format, GLenum attachment)
    		:
			m_fileName(fileName),
			m_texData(nullptr)
		{
			try
			{
			int x, y, bytesPerPixel;
			unsigned char* data = stbi_load(fileName, &x, &y, &bytesPerPixel, 4);
			if (data == nullptr)
			{
				stbi_image_free(data);
				throw IOException(std::string("Couldn' load File \"").append(fileName).append("\""));
			}
			m_texData = new TextureData(textureType, x, y, 1, &data, &filter, &pattern, &internalFormat, &format, &attachment);

			stbi_image_free(data); // free resources
			}
			catch (const std::exception& e)
			{
				BOOST_LOG_TRIVIAL(error) << e.what();
			}
		}


		CTexture::CTexture(int width, int height, unsigned char* data, TextureType textureType, GLfloat filter, GLfloat pattern, GLenum internalFormat,
			GLenum format, GLenum attachment) :
    		m_fileName("")
		{
			m_texData = new TextureData(textureType, width, height, 1, &data, &filter, &pattern, &internalFormat, &format, &attachment);
		}

		CTexture::~CTexture(){
			Dispose();
		}

		void CTexture::Dispose()
		{
			if(!m_hasDisposed)
			{
				delete m_texData;
				m_hasDisposed = true;
			}
		}

		bool CTexture::HasDisposed(){
			return m_hasDisposed;
		}

		void CTexture::Bind(unsigned int unit)const{
			if (!(unit >= 0 && unit <= 31))
			{
				BOOST_LOG_TRIVIAL(fatal) << "Invalid binding unit for texture at location h"
					<< std::hex << m_texData->m_texID << std::dec;
				assert(false);
			}
			glActiveTexture(GL_TEXTURE0 + unit);
			m_texData->Bind(0);
		}

		void CTexture::BindAsRenderTarget()
		{
			m_texData->BindAsRenderTarget();
		}
	}
