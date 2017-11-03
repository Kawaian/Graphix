#pragma once

#include <GL/GLew.h>
#include <string>

#include "Type.h"
#include "Tags.h"
#include "../Graphics/Shader.h"

namespace gpx {
namespace gl4 {
template<>
struct GLObjDelAlloc<Tags::VertexArray> {
  static void Allocate(GLTypeCT<GLType::Integer> count,
                       Tags::VertexArray::internal* names) {
    glGenVertexArrays(count, names);
  } 
  static void Delete(GLTypeCT<GLType::Integer> count, Tags::VertexArray::internal* names) {
   glDeleteVertexArrays(count, names);
  }
};
template<>
struct GLObjBind<Tags::VertexArray> {
 private:
  static Tags::VertexArray::internal _active;
 public:
  static const int ZERO_BUFFER = GL_NONE;
  static void BindRequest(Tags::VertexArray::internal name = ZERO_BUFFER) {
    glBindVertexArray(name);
  }
};
Tags::VertexArray::internal 
 GLObjBind<Tags::VertexArray>::_active = 0;

class VertexArrayAttribute {
 public:
  VertexArrayAttribute(const ShaderProgram* shader, const std::string& attribName) 
    : attrib_(attribName) {
    shader->GetAttribLocation(attribName.c_str());
  }
  template<typename T>
  void Define(unsigned location, unsigned count, unsigned stride = 0, bool normalized = false) {
    glVertexAttribPointer(location, count, GLTypeCT<T>::type,
      normalized, stride, static_cast<void*>(nullptr));
  }
  void Enable(unsigned location) {
    glEnableVertexAttribArray(location);
  }
  void Disable(unsigned location) {
    glDisableVertexAttribArray(location);
  }
 private:
   std::string attrib_;
};

//wrapper around GLObjDelAlloc & GLObjBind
class VertexArray {
 public:
  VertexArray() {
    GLObjDelAlloc<Tags::VertexArray>::Allocate(1, &handle_);
  }
  ~VertexArray() {
    GLObjDelAlloc<Tags::VertexArray>::Delete(1, &handle_);
  }
  inline void Bind() {
    GLObjBind<Tags::VertexArray>::BindRequest(handle_);
  }
  inline void UnBind() {
    GLObjBind<Tags::VertexArray>::BindRequest(
     GLObjBind<Tags::VertexArray>::ZERO_BUFFER);
  }

  VertexArray& operator=(const VertexArray& other) = delete;
  VertexArray(const VertexArray& other) = delete;
private:
  Tags::VertexArray::internal handle_ = 0;
};
} //namespace GLLayer
} //namespace Spiky