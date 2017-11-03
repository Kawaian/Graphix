#pragma once
#include <exception>
#include <stdexcept>
#include <GL/glew.h>

#define GL_ENUM_STRING(id) " # id # "

namespace gpx
{
	class RuntimeException : public std::runtime_error
	{
	public:
		explicit RuntimeException(const std::string& msg)
			:
				std::runtime_error(msg)
		{}
		explicit RuntimeException()
			:
			std::runtime_error("")
		{}
	};

	class ContextInitException : public RuntimeException
	{
	public:
		explicit ContextInitException(const std::string& msg)
			:
				RuntimeException(std::string("Context Initialization Exception : ")
							.append(msg))
		{}
		explicit ContextInitException()
			:
			RuntimeException(std::string("Context Initialization Exception : "))
		{}
	};

	class IOException : public RuntimeException
	{
	public:
		explicit IOException(const std::string& msg)
			:
				RuntimeException(std::string("IO Exception : ").append(msg))
		{}

		explicit IOException()
			:
			RuntimeException(std::string("IO Exception : "))
		{}
	};

	class GLSLParseException : public RuntimeException
	{
	public:
		explicit GLSLParseException(const std::string& msg)
			:
				RuntimeException(std::string("GLSL Syntax Error : \n\t")
							.append(msg))
		{}

		explicit GLSLParseException()
			:
			RuntimeException(std::string("GLSL Syntax Error : "))
		{}
	};

	class InvalidGLStateException : public RuntimeException
	{
	public:
		explicit InvalidGLStateException(GLuint state, const std::string& location)
			:
			RuntimeException(std::string("Invalid Opengl state passed in : \"").append(GL_ENUM_STRING(state)).append("\" at ").append(location))
		{}
		explicit InvalidGLStateException()
			:
			RuntimeException(std::string("Invalid Opengl state passed in : "))
		{}

	};

	class GLSLComponentException : public RuntimeException
	{
	public:
		explicit GLSLComponentException(const std::string& msg)
			:
			RuntimeException(std::string("GLSL Component Exception : ").append(msg))
		{}
		explicit GLSLComponentException()
			:
			RuntimeException(std::string("GLSL Component Exception"))
		{}

	};
}
