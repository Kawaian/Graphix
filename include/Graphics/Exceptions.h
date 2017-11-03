#pragma once
#include <exception>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

namespace gpx
{


	class OutOfBoundsExcpetion : public std::exception{
	public:
		const char* what() const throw() {
			return "ERROR : OUT OF BOUNDS EXCEPTION : CONTAINER IS OUT OF BOUNDS !";
		}
	};

	class NotImplementedExcpetion : public std::exception{
	public:
		const char* what() const throw() {
			return "ERROR : NOT IMPLEMENTED EXCEPTION : METHOD NOT IMPLEMENTED YET !";
		}
	};

	class TypeRegisteredTwiceException : public std::exception{
	public:
		const char* what() const throw() {
			return "ERROR : TYPE REGISTERED TWICE EXCEPTION : SAME TYPE FOUND TWICE IN CONTAINER :  WILL NOW PROBABLY STOP PROGRAM !";
		}
	};

	class UnknownTypeException : public std::exception{
	public:
		const char* what() const throw() {
			return "ERROR : UNKNOWN TYPE : THE TYPE PASSED IS UNKNOWN !";
		}
	};

	class CreatedTwiceException : public std::exception{
	public:
		const char* what() const throw() {
			return "ERROR : CREATED TWICE EXCEPTION : CREATE METHOD CALLED TWICE !";
		}
	};

	class NotLoadedException : public std::exception{
	public:
		const char* what() const throw() {
			return "ERROR : NOT LOADED EXCEPTION : NECCESSARY COMPONENT HAS NOT BEEN LOADED !";
		}
	};

	class SDL_InitException : public std::exception
	{
	public:
		const char* what() const throw() {
			return std::string("ERROR INITIALIZING SDL :", SDL_GetError()).c_str();
		}
	};
}