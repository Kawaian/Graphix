#include "../include/Graphics/SDL_Handle.h"
#include <boost/log/trivial.hpp>
#include "../include/Core/Exceptions.h"

using namespace gpx;

unsigned SDL_Handle::m_refs = 0;

SDL_Handle::SDL_Handle(int& status){
    ++m_refs;
    if(m_refs == 1)
    {
		try {
			if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != 0)
				throw ContextInitException(std::string("SDL context intialization failed : ", SDL_GetError()));
			BOOST_LOG_TRIVIAL(info) << "Created Windowing Contexts";
    }
		catch (std::exception& e) {
			BOOST_LOG_TRIVIAL(error) << e.what();
      status = -1;
    }
    status = 0;
	}
}
SDL_Handle::SDL_Handle() {
  ++m_refs;
  if (m_refs == 1)
  {
    try {
      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != 0)
        throw ContextInitException(std::string("SDL context intialization failed : ", SDL_GetError()));
      BOOST_LOG_TRIVIAL(info) << "Created Windowing Contexts";
    }
    catch (std::exception& e) {
      BOOST_LOG_TRIVIAL(error) << e.what();
    }
  }
}
SDL_Handle::~SDL_Handle(){
    if(--m_refs == 0)
    {
        SDL_Quit();
		BOOST_LOG_TRIVIAL(info) << "Disposed Windowing Contexts";
	}
}
