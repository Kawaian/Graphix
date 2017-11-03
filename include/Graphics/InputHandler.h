#pragma once
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../include/Graphics/SDL_Handle.h"

namespace gpx
{
	class InputHandler
	{
	public:

		InputHandler();
		virtual ~InputHandler(){}

		void Update();
		bool CloseRequested() const { return m_closeRequested; }
		glm::ivec2 GetMousePosition() const { return m_mousePosition; }
		bool KeyDown(SDL_Scancode key) const { return m_keyStates[key] == 0 ? false : true; }
		bool MouseDown() { return m_mouseDown; }

	private:
		static InputHandler _object;
		bool m_closeRequested = false;
		SDL_Handle m_handle;
		glm::ivec2 m_mousePosition = glm::ivec2(0, 0);
		bool m_mouseDown = false;
		const Uint8* m_keyStates = nullptr;
	};

}