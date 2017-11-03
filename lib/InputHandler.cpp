#include "../include/Graphics/InputHandler.h"

using namespace gpx;

InputHandler::InputHandler(){
    m_handle = SDL_Handle();
}

void InputHandler::Update(){
    m_keyStates = SDL_GetKeyboardState(nullptr);

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                m_closeRequested = true;
                break;
            case SDL_MOUSEMOTION:
                m_mousePosition = glm::uvec2(event.motion.x, event.motion.y);
                break;
			case SDL_MOUSEBUTTONDOWN:
				m_mouseDown = true;
				break;
			case SDL_MOUSEBUTTONUP:
				m_mouseDown = false;
				break;
		}
    }
}


