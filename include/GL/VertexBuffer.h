#pragma once

#include <memory>
#include <GL/glew.h>

#include "Type.h"
#include "Tags.h"

namespace gpx {
namespace gl4 {
template<>
struct GLObjDelAlloc<Tags::VertexBuffer> {
  static void Allocate(GLTypeCT<GLType::Integer> count,
                       Tags::VertexBuffer::internal* names) {
    glGenBuffers(count, names);
  } 
  static void Delete(GLTypeCT<GLType::Integer> count, Tags::VertexBuffer::internal* names) {
   glDeleteBuffers(count, names);
  }
};
template<>
struct GLObjBind<Tags::VertexBuffer> {
 private:
  static Tags::VertexBuffer::internal _active;
 public:
  static const int ZERO_BUFFER = GL_NONE;
  static void BindRequest(Tags::VertexBuffer::internal name = ZERO_BUFFER) {
 //   if(_active != name) {
      glBindBuffer(GL_ARRAY_BUFFER, name);
 //     _active = name;
 //   }
  }
};
Tags::VertexBuffer::internal 
 GLObjBind<Tags::VertexBuffer>::_active = 0;

//wrapper around GLObjDelAlloc & GLObjBind
class VertexBuffer {
 public:
  explicit VertexBuffer() {
    GLObjDelAlloc<Tags::VertexBuffer>::Allocate(1, &handle_);
  }
  ~VertexBuffer() {
    GLObjDelAlloc<Tags::VertexBuffer>::Delete(1, &handle_);
  }
  inline void Bind() {
    GLObjBind<Tags::VertexBuffer>::BindRequest(handle_);
  }
  inline void UnBind() {
    GLObjBind<Tags::VertexBuffer>::BindRequest(
     GLObjBind<Tags::VertexBuffer>::ZERO_BUFFER);
  }
  template<typename T>
  static void BufferData(const GLTypeCT<GLType::Integer> count, GLTypeCT<T>* data) {
    glBufferData(GL_ARRAY_BUFFER, count *
      data->stride, data, GL_STATIC_DRAW);
  }
  
  VertexBuffer& operator=(const VertexBuffer& other) = delete;
  VertexBuffer(const VertexBuffer& other) = delete;
 protected:
  Tags::VertexBuffer::internal handle_;
};

} //namespace GLLayer
} //namespace Spiky
