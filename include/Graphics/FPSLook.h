#pragma once
#include "../include/Graphics/Camera.h"
#include "../include/Graphics/InputHandler.h"
#include "../include/Graphics/GraphicsDisplay.h"

namespace gpx
{
	namespace util
	{

		inline void ApplyFreeFPSLook(Camera& camera, InputHandler& input, GraphicsDisplay& window, float elapsed,
			const float& CAMERA_MOVE_SPEED, const float MOUSE_SENSITIVITY, const glm::ivec2 MOUSE_TRAP){

			if (input.KeyDown(SDL_SCANCODE_W))
				camera.OffSetPosition(elapsed * CAMERA_MOVE_SPEED * camera.GetForward());
			else if (input.KeyDown(SDL_SCANCODE_S))
				camera.OffSetPosition(elapsed * CAMERA_MOVE_SPEED * -camera.GetForward());
			if (input.KeyDown(SDL_SCANCODE_D))
				camera.OffSetPosition(elapsed * CAMERA_MOVE_SPEED * camera.GetRight());
			else if (input.KeyDown(SDL_SCANCODE_A))
				camera.OffSetPosition(elapsed * CAMERA_MOVE_SPEED * -camera.GetRight());
			glm::ivec2 mouseVeloc = input.GetMousePosition() - MOUSE_TRAP;
			bool rotY = mouseVeloc[0] != 0;
			bool rotX = mouseVeloc[1] != 0;
			if (rotY)
				camera.OffSetOrientation(0.0, glm::radians(mouseVeloc[0] * MOUSE_SENSITIVITY));
			if (rotX)
				camera.OffSetOrientation(glm::radians(mouseVeloc[1] * MOUSE_SENSITIVITY), 0.0);

			if (rotX || rotY)
				window.SetMousePosition(MOUSE_TRAP[0], MOUSE_TRAP[1]);
      
      window.SetMouseVisible(false);
		}
	}

}