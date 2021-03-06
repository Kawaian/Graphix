#pragma once

#include <GL/GLew.h>
#include <string>

#include "Type.h"
#include "Tags.h"
#include "../Graphics/Shader.h"

namespace gpx {
  namespace gl4 {
    template<>
    struct GLObjDelAlloc<Tags::UniformBuffer> {
      static void Allocate(GLTypeCT<GLType::Integer> count,
        Tags::UniformBuffer::internal* names) {
        glGenBuffers(count, names);
      }
      static void Delete(GLTypeCT<GLType::Integer> count, Tags::UniformBuffer::internal* names) {
        glDeleteBuffers(count, names);
      }
    };
    template<>
    struct GLObjBind<Tags::UniformBuffer> {
    private:
      static Tags::UniformBuffer::internal _active;
    public:
      static const int ZERO_BUFFER = GL_NONE;
      static void BindRequest(Tags::UniformBuffer::internal name = ZERO_BUFFER) {
        glBindBuffer(GL_UNIFORM_BUFFER, name);
      }
    };
    Tags::UniformBuffer::internal
      GLObjBind<Tags::UniformBuffer>::_active = 0;

    //wrapper around GLObjDelAlloc & GLObjBind
    class UniformBuffer {
    public:
      UniformBuffer() {
        GLObjDelAlloc<Tags::UniformBuffer>::Allocate(1, &handle_);
      }
      ~UniformBuffer() {
        GLObjDelAlloc<Tags::UniformBuffer>::Delete(1, &handle_);
      }
      inline void Bind() {
        GLObjBind<Tags::UniformBuffer>::BindRequest(handle_);
      }
      inline void UnBind() {
        GLObjBind<Tags::UniformBuffer>::BindRequest(
          GLObjBind<Tags::UniformBuffer>::ZERO_BUFFER);
      }

      void BindUniformBlock(const std::unique_ptr<
        ShaderProgram>& sh, const char* blockName){
        GLuint uniformblockindex =
          glGetUniformBlockIndex(sh->GetProgram(), blockName);
        glUniformBlockBinding(sh->GetProgram(), uniformblockindex, 0);

      }

      template<typename T>
      static void BufferData(const GLTypeCT<GLType::Integer> count,
        GLTypeCT<T>* data) {
        glBufferData(GL_UNIFORM_BUFFER, count *
          data->stride, data, GL_STATIC_DRAW);
      }
      
      static void EmptyBufferData(GLsizei dataSize) {
        glBufferData(GL_UNIFORM_BUFFER, dataSize, nullptr, GL_STATIC_DRAW);
      }

      void BufferRange( 
        unsigned index, GLintptr offset, GLsizeiptr size) {
        glBindBufferRange(GL_UNIFORM_BUFFER, index, handle_, offset, size);
      }

      UniformBuffer& operator=(const UniformBuffer& other) = delete;
      UniformBuffer(const UniformBuffer& other) = delete;
    private:
      Tags::UniformBuffer::internal handle_ = 0;
    };
  } //namespace GLLayer
} //namespace Spiky