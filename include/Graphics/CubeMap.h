/*
 * CubeMap.h
 *
 *  Created on: Aug 9, 2015
 *      Author: mattmatt
 */

#pragma once

#include <GL/glew.h>
#include <string>

namespace gpx
{


	static const GLenum GL_TEXTURE_CUBE_MAP_TYPES[6] =
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	};

	class CubeMap {
	public:
		CubeMap(
			const char* posXFile,
			const char* negXFile,
			const char* posYFile,
			const char* negYFile,
			const char* posZFile,
			const char* negZFile
			);

		~CubeMap();

		void Bind(GLenum unit = 0);

		void operator=(const CubeMap& other) = delete;
		CubeMap(const CubeMap& other) = delete;

	private:
		GLuint m_texture;
		std::string m_fileNames[6];
	};
}