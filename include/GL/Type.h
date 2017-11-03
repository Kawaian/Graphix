#pragma once

#include <vector>
#include <GL/glew.h>
#include <type_traits>

namespace gpx {
namespace gl4 {
namespace GLType {
  typedef GLbyte     Byte;
  typedef GLubyte    UByte;
  typedef GLshort    Short;
  typedef GLushort   UShort;
  typedef GLint      Integer;
  typedef GLuint     UInteger;
  typedef GLint64    Integer64;
  typedef GLuint64   UInteger64;
  typedef GLfloat    Float;
  typedef GLboolean  Boolean;
} //namespace GLType

template<typename T, unsigned N = 1>
struct GLTypeCT {
  typedef T internal;
  GLTypeCT(internal v)
    : value(v) {
  }
  enum { type = GLTypeCT<T>::type };
  enum { count = N };
  enum { stride = sizeof(GLTypeCT<T>::type*N) };
  internal value;
  operator internal() const {
    return value;
  }
};
template<>
struct GLTypeCT<GLType::Byte> {
  typedef GLType::Byte internal;
  GLTypeCT(internal v = (internal)(0))
    : value(v) {
  }
  enum { type = GL_BYTE };
  enum { count = 1 };
  enum { stride = sizeof(signed char) };
  internal value;
  operator internal() const {
    return value;
  }
};
template<>
struct GLTypeCT<GLType::UByte> {
  typedef GLType::UByte internal;
  GLTypeCT(internal v = (internal)(0))
    : value(v) {
  }
  enum { type = GL_UNSIGNED_BYTE };
  enum { count = 1 };
  enum { stride = sizeof(unsigned char) };
  internal value;
  explicit operator internal() const {
    return value;
  }
};
template<>
struct GLTypeCT<GLType::Short> {
  typedef GLType::Short internal;
  GLTypeCT(internal v = (internal)(0))
    : value(v) {
  }
  enum { type = GL_SHORT };
  enum { count = 1 };
  enum { stride = sizeof(short) };
  internal value;
  operator internal() const {
    return value;
  }
};
template<>
struct GLTypeCT<GLType::UShort> {
  typedef GLType::UShort internal;
  GLTypeCT(internal v = (internal)(0))
    : value(v) {
  }
  enum { type = GL_UNSIGNED_SHORT };
  enum { count = 1 };
  enum { stride = sizeof(unsigned short) };
  internal value;
  operator internal() const {
    return value;
  }
};
template<>
struct GLTypeCT<GLType::Integer> {
  typedef GLType::Integer internal;
  GLTypeCT(internal v = (internal)(0))
    : value(v) {
  }
  enum { type = GL_INT };
  enum { count = 1 };
  enum { stride = sizeof(int) };
  internal value;
  operator internal() const {
    return value;
  }
};
template<>
struct GLTypeCT<GLType::UInteger> {
  typedef GLType::UInteger internal;
  GLTypeCT(internal v = (internal)(0))
    : value(v) {
  }
  enum { type = GL_UNSIGNED_INT };
  enum { count = 1 };
  enum { stride = sizeof(unsigned int) };
  internal value;
  operator internal() const {
    return value;
  }
};
template<>
struct GLTypeCT<GLType::Integer64> {
  typedef GLType::Integer64 internal;
  GLTypeCT(internal v = (internal)(0))
    : value(v) {
  }
  enum { type = GL_INT };
  enum { count = 1 };
  enum { stride = sizeof(long long) };
  internal value;
  operator internal() const {
    return value;
  }
};
template<>
struct GLTypeCT<GLType::UInteger64> {
  typedef GLType::UInteger64 internal;
  GLTypeCT(internal v = (internal)(0))
    : value(v) {
  }
  enum { type = GL_UNSIGNED_INT };
  enum { count = 1 };
  enum { stride = sizeof(unsigned long long) };
  internal value;
  operator internal() const {
    return value;
  }
};
template<>
struct GLTypeCT<GLType::Float> {
  typedef GLType::Float internal; 
  GLTypeCT(internal v = (internal)(0))
   : value(v) {
  }
  enum { type = GL_FLOAT };
  enum { count = 1 };
  enum { stride = sizeof(float) };
  internal value;
  operator internal() const {
    return value; 
  }
};
} //namespace GLLayer
} //namespace gpx