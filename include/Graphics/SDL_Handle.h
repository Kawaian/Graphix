#pragma once
#include <SDL2/SDL.h>

namespace gpx
{
	class SDL_Handle
	{
	public:
    SDL_Handle();
    SDL_Handle(int& status);
		~SDL_Handle();
		inline unsigned GetRefs() { return m_refs; }
	private:
		static unsigned m_refs;
	};
}