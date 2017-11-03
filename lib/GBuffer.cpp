#include "../include/Graphics/GBuffer.h"
#include <boost/log/trivial.hpp>

namespace gpx
{
    GBuffer::GBuffer(unsigned width, unsigned height)
        : m_FBO(0), m_depthTexture(0), m_finalTexture(0), m_hasDisposed(false)
    {
    	for (unsigned i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
    		m_textures[0] = 0;

        Init(width, height);
    }

    GBuffer::~GBuffer(){
        Dispose();
    }

    void GBuffer::Dispose(){
        if(!m_hasDisposed)
        {
            if(m_FBO != 0)
                glDeleteFramebuffers(1, &m_FBO);
            if(m_textures[0] != 0)
                glDeleteTextures(sizeof(m_textures) / sizeof(m_textures[0]),
                    m_textures);
            if (m_depthTexture != 0)
                glDeleteTextures(1, &m_depthTexture);
			if (m_finalTexture != 0)
				glDeleteTextures(1, &m_finalTexture);
            m_hasDisposed = true;
        }
    }

    bool GBuffer::Init(unsigned width, unsigned height){
        //create fbo
        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);

        //create gbuffer textures
        glGenTextures(sizeof(m_textures) / sizeof(m_textures[0]), m_textures);
        
	    	glGenTextures(1, &m_depthTexture);
		    glGenTextures(1, &m_finalTexture);

        for(unsigned i = 0; i < sizeof(m_textures) / sizeof(m_textures[0]); ++i){
            glBindTexture(GL_TEXTURE_2D, m_textures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
                width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
        }
        //depth
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8,
			width, height, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, nullptr);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
            GL_TEXTURE_2D, m_depthTexture, 0);

		//final
		glBindTexture(GL_TEXTURE_2D, m_finalTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F,
			width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D,
			m_finalTexture, 0);
		/*
        GLenum drawBuffers[] =
        {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2,
            GL_COLOR_ATTACHMENT3
        };
        glDrawBuffers(sizeof(drawBuffers) / sizeof(drawBuffers[0]),
            drawBuffers);
		*/

        //error checking...
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE){
            BOOST_LOG_TRIVIAL(error) << "Failed to build FrameBuffer : glError : h" 
                                << std::hex << status << std::dec;
            return false;
        }

        //back to default FBO
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        return true;
    }

	void GBuffer::BindForGeomPass()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);

		GLenum drawBuffers[] =
		{
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2
		};
		glDrawBuffers(sizeof(drawBuffers) / sizeof(drawBuffers[0]),
			drawBuffers);
	}

	void GBuffer::BindForLightPass()
	{
		glDrawBuffer(GL_COLOR_ATTACHMENT3);

		for (unsigned i = 0; i < sizeof(m_textures) / sizeof(m_textures[0]); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
		}

	}

	/*
	//TODO deprecated
    void GBuffer::BindForOutput()
	{
		
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        for(unsigned i = 0; i < sizeof(m_textures) / sizeof(m_textures[0]); ++i){
        	glActiveTexture(GL_TEXTURE0  + i);
        	glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
        }
		
        //glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);//TODO switch this
    }

	//TODO deprecated
    void GBuffer::BindForInput()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);

    }
	*/
	void GBuffer::StartFrame()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
		glDrawBuffer(GL_COLOR_ATTACHMENT3);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GBuffer::BindForStencilPass()
	{
		glDrawBuffer(GL_NONE);
	}

	void GBuffer::BindForFinalPass()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
		glReadBuffer(GL_COLOR_ATTACHMENT3);
	}

    void GBuffer::BindFinalTexture()
    {
		  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
		  glActiveTexture(GL_TEXTURE0);
		  glBindTexture(GL_TEXTURE_2D, m_finalTexture);
    }

    void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE texType)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + texType);
    }
}


