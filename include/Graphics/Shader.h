#pragma once

#include <GL/glew.h>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "Component.h"
#include "Exceptions.h"
#include "../core/Types.h"
#include "../Core/OSConfig.h"
#define GL_NUM_SHADER_TYPES 3
#define GL_NO_UNIFORM -1

namespace gpx {
enum class ShaderType { Vertex, Fragment, Geometry };

class ShaderProgram : public Component {
 public:
  ShaderProgram();
  ShaderProgram(const char* vertexPath, const char* fragmentPath);
  ShaderProgram(const char* vertexPath, const char* fragmentPath,
                const char* geometryPath);
  virtual ~ShaderProgram();

  void AttachShaderFile(ShaderType type, const std::string& fileName);
  void AttachShaderSource(ShaderType type, const std::string& src);
  void Create();
  GLuint GetProgram() const;
  GLuint GetAttribLocation(const char* attrib) const;
  GLuint GetUniformLocation(const char* unif) const;
  void UpdateUniformMatrix4f(const char* unif, const glm::mat4& val,
                             GLboolean inverse = false) const;
  void UpdateUniformMatrix3f(const char* unif, const glm::mat3& val,
                             GLboolean inverse = false) const;
  void UpdateUniformMatrix2f(const char* unif, const glm::mat2& val,
                             GLboolean inverse = false) const;
  void Bind() const override;
  void UnBind() const override;
  bool IsBound() const override;
  void Dispose() override;

#define __SHADER_DEFINE_HEADER(OGL_TYPE, TYPE_SUFFIX)                        \
  void UpdateUniform1##TYPE_SUFFIX(const GLchar* name, OGL_TYPE v0) const;   \
  void UpdateUniform2##TYPE_SUFFIX(const GLchar* name, OGL_TYPE v0,          \
                                   OGL_TYPE v1) const;                       \
  void UpdateUniform3##TYPE_SUFFIX(const GLchar* name, OGL_TYPE v0,          \
                                   OGL_TYPE v1, OGL_TYPE v2) const;          \
  void UpdateUniform4##TYPE_SUFFIX(const GLchar* name, OGL_TYPE v0,          \
                                   OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3)    \
      const;                                                                 \
  void UpdateUniform1##TYPE_SUFFIX##v(const GLchar* name, const OGL_TYPE* v, \
                                      GLsizei count = 1) const;              \
  void UpdateUniform2##TYPE_SUFFIX##v(const GLchar* name, const OGL_TYPE* v, \
                                      GLsizei count = 1) const;              \
  void UpdateUniform3##TYPE_SUFFIX##v(const GLchar* name, const OGL_TYPE* v, \
                                      GLsizei count = 1) const;              \
  void UpdateUniform4##TYPE_SUFFIX##v(const GLchar* name, const OGL_TYPE* v, \
                                      GLsizei count = 1) const;

  __SHADER_DEFINE_HEADER(GLfloat, f);
  __SHADER_DEFINE_HEADER(GLdouble, d);
  __SHADER_DEFINE_HEADER(GLint, i);
  __SHADER_DEFINE_HEADER(GLuint, ui);

#undef __ALPHA_SHADER_PROGRAM_UNIFORM_DECLERATION

  ShaderProgram(ShaderProgram&& other);

 private:
  INTERNAL std::string LoadShaderSource(const std::string& fileName);

  GLOBAL const unsigned VERTEX_LOC = 0;
  GLOBAL const unsigned FRAGMENT_LOC = 1;
  GLOBAL const unsigned GEOMETRY_LOC = 2;
  int m_numShaders;
  GLuint m_object;
  bool m_hasCreated;
  bool m_hasDisposed;
  GLuint m_shadersObjects[GL_NUM_SHADER_TYPES];
  mutable std::map<std::string, GLuint> m_attribLocations;
  mutable std::map<std::string, GLuint> m_unifLocations;
};

typedef std::shared_ptr<ShaderProgram> ShaderSmtPtr;

ShaderSmtPtr CreateShader();
ShaderSmtPtr CreateShader(const char* vertexPath, const char* fragmentPath);
ShaderSmtPtr CreateShader(const char* vertexPath, const char* fragmentPath,
                          const char* geometryPath);
}
