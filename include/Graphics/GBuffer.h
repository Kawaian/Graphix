#pragma once
#include <GL/glew.h>

namespace gpx
{

	class GBuffer
	{
	public:
		enum GBUFFER_TEXTURE_TYPE
		{
			GBUFFER_TEXTURE_TYPE_POSITION,
			GBUFFER_TEXTURE_TYPE_ALBEDO,
			GBUFFER_TEXTURE_TYPE_NORMAL,
			GBUFFER_TEXTURE_TYPE_SPECULAR,
			GBUFFER_NUM_TEXTURES

		};
		GBuffer(unsigned width, unsigned height);
		~GBuffer();
		void Dispose();
		void StartFrame();
		void BindForGeomPass();
		void BindForLightPass();
		void BindForStencilPass();
		void BindForFinalPass();
		void BindFinalTexture();
		void BindForInput();
		void BindForOutput();
		void SetReadBuffer(GBUFFER_TEXTURE_TYPE textureType);

		GBuffer& operator=(const GBuffer& other) = delete;
		GBuffer(GBuffer& other) = delete;
	private:
		bool Init(unsigned width, unsigned height);
		GLuint m_FBO;
		GLuint m_textures[GBUFFER_NUM_TEXTURES];
		GLuint m_depthTexture;
		GLuint m_finalTexture;
		bool m_hasDisposed;
	};
}