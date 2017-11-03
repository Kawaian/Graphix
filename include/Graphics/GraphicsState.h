#pragma once

#include <glm/glm.hpp>
#include "Light.h"

namespace gpx
{

	struct GraphicsState
	{
		//VS PASS
		glm::mat4 MVP;
		glm::mat4 model;
		glm::mat3 normalMatrix;
		//FS PASS
		glm::vec3 cameraPosition;
		PointLight light;
	};
}