/*
 * CubeMap.cpp
 *
 *  Created on: Aug 9, 2015
 *      Author: mattmatt
 */

#include <cassert>
#include "../include/Graphics/CubeMap.h"
#include "../include/Graphics/libs/stb_image.h"
#include "../include/Graphics/LogManager.h"

using namespace gpx;

CubeMap::CubeMap	(
						const char* posXFile,
						const char* negXFile,
						const char* posYFile,
						const char* negYFile,
						const char* posZFile,
						const char* negZFile
					)
	:	m_texture(0)
{

	m_fileNames[0] = posXFile;
	m_fileNames[1] = negXFile;
	m_fileNames[2] = posYFile;
	m_fileNames[3] = negYFile;
	m_fileNames[4] = posZFile;
	m_fileNames[5] = negZFile;


	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	for (unsigned i = 0; i < (sizeof(GL_TEXTURE_CUBE_MAP_TYPES) / sizeof(GL_TEXTURE_CUBE_MAP_TYPES[0])); ++i){
		int w, h, bytesPerPixel;
		unsigned char*  data = stbi_load((m_fileNames[i].c_str()), &w, &h, &bytesPerPixel, 4);

		if(data == nullptr){
			LOG_ERROR_INFO("CubeMap(Texture)", "Failed to load Texture from file", m_fileNames[i]);
				assert(0);
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_TYPES[i], 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	LOG_INFO("CubeMap(Texture)",  ": Has init successfully : ID is", m_texture );
}

CubeMap::~CubeMap()
{
	if(m_texture != 0){
		glDeleteTextures(1, &m_texture);
		LOG_INFO("CubeMap(Texture)",  ": Delete texture object successfully : ID was", m_texture );
	}
}

void CubeMap::Bind(GLenum unit){
	assert(unit >= 0 && unit <= 31);
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}


