#include <fstream>
#include <assert.h>
#include <boost/log/trivial.hpp>
#include "../include/Graphics/Shader.h"
#include "../include/Core/Exceptions.h"

namespace gpx {
ShaderSmtPtr CreateShader() { return std::make_shared<ShaderProgram>(); }

ShaderSmtPtr CreateShader(const char* vertexPath, const char* fragmentPath) {
  return std::make_shared<ShaderProgram>(vertexPath, fragmentPath);
}

ShaderSmtPtr CreateShader(const char* vertexPath, const char* fragmentPath,
                          const char* geometryPath) {
  return std::make_shared<ShaderProgram>(vertexPath, fragmentPath,
                                         geometryPath);
}
ShaderProgram::ShaderProgram()
    : Component::Component(),
      m_numShaders(0),
      m_object(0),
      m_hasCreated(false),
      m_hasDisposed(false){
  m_shadersObjects[VERTEX_LOC] = 0;
  m_shadersObjects[FRAGMENT_LOC] = 0;
  m_shadersObjects[GEOMETRY_LOC] = 0;
  m_attribLocations.clear();
  m_unifLocations.clear();
}

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
    : ShaderProgram() {
  this->AttachShaderFile(ShaderType::Vertex, vertexPath);
  this->AttachShaderFile(ShaderType::Fragment, fragmentPath);
  this->Create();
}

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath,
                             const char* geometryPath)
    : ShaderProgram() {
  this->AttachShaderFile(ShaderType::Vertex, vertexPath);
  this->AttachShaderFile(ShaderType::Fragment, fragmentPath);
  this->AttachShaderFile(ShaderType::Geometry, geometryPath);
  this->Create();
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) {
  this->m_object = other.m_object;
  other.m_object = 0;
  this->m_numShaders = other.m_numShaders;
  this->m_hasCreated = other.m_hasCreated;
  this->m_hasDisposed = other.m_hasDisposed;
  other.m_hasDisposed = true;
  for (unsigned i = 0; i < GL_NUM_SHADER_TYPES; ++i) {
    this->m_shadersObjects[i] = other.m_shadersObjects[i];
    other.m_shadersObjects[i] = 0;
  }
  this->m_attribLocations = std::move(other.m_attribLocations);
  this->m_unifLocations = std::move(other.m_unifLocations);
  this->m_unifLocations = std::move(other.m_unifLocations);
}

ShaderProgram::~ShaderProgram() { Dispose(); }

void ShaderProgram::AttachShaderFile(ShaderType which,
                                     const std::string& fileName) {
  std::string shaderSource = LoadShaderSource(fileName);
  AttachShaderSource(which, shaderSource);
}

void ShaderProgram::AttachShaderSource(ShaderType type,
                                       const std::string& text) {
  try {
    switch (type) {
      case ShaderType::Vertex:
        if (glIsShader(m_shadersObjects[VERTEX_LOC]) == GL_TRUE)
          throw TypeRegisteredTwiceException();
        break;
      case ShaderType::Fragment:
        if (glIsShader(m_shadersObjects[FRAGMENT_LOC]) == GL_TRUE)
          throw TypeRegisteredTwiceException();
        break;
      case ShaderType::Geometry:
        if (glIsShader(m_shadersObjects[GEOMETRY_LOC]) == GL_TRUE)
          throw TypeRegisteredTwiceException();
        break;
      default:
        break;
    }

    GLuint shader = 0;

    if (type == ShaderType::Vertex)
      shader = glCreateShader(GL_VERTEX_SHADER);
    else if (type == ShaderType::Fragment)
      shader = glCreateShader(GL_FRAGMENT_SHADER);
    else if (type == ShaderType::Geometry)
      shader = glCreateShader(GL_GEOMETRY_SHADER);

    const char* cstr = text.c_str();
    glShaderSource(shader, 1, &cstr, nullptr);

    // compile + check shader load status
    GLint status;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
      GLint infoLogSize;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogSize);
      GLchar* infoLog = new GLchar[infoLogSize];
      glGetShaderInfoLog(shader, infoLogSize, nullptr, infoLog);
      std::string strInfoLog = infoLog;
      delete[] infoLog;  // no throw between new and delete

      switch (type) {
        case ShaderType::Vertex:
          throw GLSLParseException(
              std::string("In Vertex-Shader").append(strInfoLog));
          break;
        case ShaderType::Fragment:
          throw GLSLParseException(
              std::string("In Fragment-Shader").append(strInfoLog));
          break;
        case ShaderType::Geometry:
          throw GLSLParseException(
              std::string("In Geometry-Shader").append(strInfoLog));
          break;
      }
    }

    if (m_numShaders >= 4) {
      throw OutOfBoundsExcpetion();
    }
    m_shadersObjects[m_numShaders++] = shader;
  } catch (std::exception& e) {
    BOOST_LOG_TRIVIAL(error) << e.what();
  }
}

void ShaderProgram::Create() {
  try {
    if (glIsProgram(m_object) == GL_TRUE) {
      this->Dispose();
      BOOST_LOG_TRIVIAL(debug) << "Rebuilding Shader at location h" << std::hex
                               << m_object << std::dec;
      glDeleteProgram(m_object);
    }

    m_object = glCreateProgram();

    m_hasCreated = true;

    if (m_shadersObjects[VERTEX_LOC] != 0)
      glAttachShader(m_object, m_shadersObjects[VERTEX_LOC]);
    else
      throw GLSLComponentException(
          "Vertex-Shader Compoment hasn't been loaded");
    if (m_shadersObjects[FRAGMENT_LOC] != 0)
      glAttachShader(m_object, m_shadersObjects[FRAGMENT_LOC]);
    else
      throw GLSLComponentException(
          "Fragment-Shader Compoment hasn't been loaded");
    if (m_shadersObjects[GEOMETRY_LOC] != 0)
      glAttachShader(m_object, m_shadersObjects[GEOMETRY_LOC]);

    // link stage
    GLint status;
    glLinkProgram(m_object);
    glGetProgramiv(m_object, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
      GLint infoLogSize;
      glGetProgramiv(m_object, GL_INFO_LOG_LENGTH, &infoLogSize);
      GLchar* infoLog = new GLchar[infoLogSize];
      glGetProgramInfoLog(m_object, infoLogSize, nullptr, infoLog);
      delete[] infoLog;
    }

    BOOST_LOG_TRIVIAL(trace) << "Building Shader at location h" << std::hex
                             << m_object << std::dec;

    glDetachShader(m_object, m_shadersObjects[VERTEX_LOC]);
    glDetachShader(m_object, m_shadersObjects[FRAGMENT_LOC]);
    glDetachShader(m_object, m_shadersObjects[GEOMETRY_LOC]);

    glDeleteShader(m_shadersObjects[VERTEX_LOC]);
    glDeleteShader(m_shadersObjects[FRAGMENT_LOC]);
    glDeleteShader(m_shadersObjects[GEOMETRY_LOC]);
  } catch (const std::exception& e) {
    BOOST_LOG_TRIVIAL(error) << e.what();
#ifdef SPIKY_IGNORE_FATAL
    BOOST_LOG_TRIVIAL(fatal) << "Warning : not all components have been loaded "
                                "for shader at location  h"
                             << m_object
                             << "Undefined Behaviour is to be expected !";
#else
    BOOST_LOG_TRIVIAL(fatal) << "Warning : not all components have been loaded "
                                "for shader at location  h"
                             << m_object;

    assert(false);
#endif
  }
}

void ShaderProgram::Bind() const {
  if (!IsBound()) glUseProgram(m_object);
}

void ShaderProgram::UnBind() const {
  if (IsBound()) glUseProgram(0);
}

bool ShaderProgram::IsBound() const {
  GLint currentProgram = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
  return (currentProgram == (GLint)m_object);
}

GLuint ShaderProgram::GetAttribLocation(const char* attrib) const {
  auto it = m_attribLocations.find(attrib);
  if (it == m_attribLocations.end()) {
    GLuint loc = glGetAttribLocation(m_object, attrib);
    if (loc == static_cast<unsigned>(-1)) {
      BOOST_LOG_TRIVIAL(warning)
          << "ShaderProgram at location h" << std::hex << m_object << std::dec
          << " uses invalid attribute location at \"" << attrib << "\"";
    }
    it = m_attribLocations.insert({attrib, loc}).first;
  }
  return it->second;
}

GLuint ShaderProgram::GetUniformLocation(const char* unif) const {
  auto it = m_unifLocations.find(unif);
  if (it == m_unifLocations.end()) {
    GLuint loc = glGetUniformLocation(m_object, unif);
    if (loc == static_cast<unsigned>(-1)) {
      BOOST_LOG_TRIVIAL(warning)
          << "ShaderProgram at location h" << std::hex << m_object << std::dec
          << " uses invalid uniform location at\"" << unif << "\"";
    }
    it = m_unifLocations.insert({unif, loc}).first;
  }
  return it->second;
}

GLuint ShaderProgram::GetProgram() const { return m_object; }

void ShaderProgram::UpdateUniformMatrix4f(const char* unif,
                                          const glm::mat4& val,
                                          GLboolean inverse) const {
  glUniformMatrix4fv(this->GetUniformLocation(unif), 1, inverse,
                     glm::value_ptr(val));
}

void ShaderProgram::UpdateUniformMatrix3f(const char* unif,
                                          const glm::mat3& val,
                                          GLboolean inverse) const {
  glUniformMatrix3fv(this->GetUniformLocation(unif), 1, inverse,
                     glm::value_ptr(val));
}

void ShaderProgram::UpdateUniformMatrix2f(const char* unif,
                                          const glm::mat2& val,
                                          GLboolean inverse) const {
  glUniformMatrix2fv(this->GetUniformLocation(unif), 1, inverse,
                     glm::value_ptr(val));
}

#define __ALPHA_SHADER_PROGRAM_UNIFORM_IMPLEMENTATION(OGL_TYPE, TYPE_SUFFIX)  \
  void ShaderProgram::UpdateUniform1##TYPE_SUFFIX(const GLchar* name,         \
                                                  OGL_TYPE v0) const {        \
    glUniform1##TYPE_SUFFIX(this->GetUniformLocation(name), v0);              \
  }                                                                           \
                                                                              \
  void ShaderProgram::UpdateUniform2##TYPE_SUFFIX(                            \
      const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) const {                   \
    glUniform2##TYPE_SUFFIX(this->GetUniformLocation(name), v0, v1);          \
  }                                                                           \
                                                                              \
  void ShaderProgram::UpdateUniform3##TYPE_SUFFIX(                            \
      const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) const {      \
    glUniform3##TYPE_SUFFIX(this->GetUniformLocation(name), v0, v1, v2);      \
  }                                                                           \
                                                                              \
  void ShaderProgram::UpdateUniform4##TYPE_SUFFIX(                            \
      const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
      const {                                                                 \
    glUniform4##TYPE_SUFFIX(this->GetUniformLocation(name), v0, v1, v2, v3);  \
  }                                                                           \
                                                                              \
  void ShaderProgram::UpdateUniform1##TYPE_SUFFIX##v(                         \
      const GLchar* name, const OGL_TYPE* v, GLsizei count)                   \
      const /** sizei=1 by default */                                         \
  {                                                                           \
    glUniform1##TYPE_SUFFIX##v(this->GetUniformLocation(name), count, v);     \
  }                                                                           \
                                                                              \
  void ShaderProgram::UpdateUniform2##TYPE_SUFFIX##v(                         \
      const GLchar* name, const OGL_TYPE* v, GLsizei count) const {           \
    glUniform2##TYPE_SUFFIX##v(this->GetUniformLocation(name), count, v);     \
  }                                                                           \
                                                                              \
  void ShaderProgram::UpdateUniform3##TYPE_SUFFIX##v(                         \
      const GLchar* name, const OGL_TYPE* v, GLsizei count) const {           \
    glUniform3##TYPE_SUFFIX##v(this->GetUniformLocation(name), count, v);     \
  }                                                                           \
                                                                              \
  void ShaderProgram::UpdateUniform4##TYPE_SUFFIX##v(                         \
      const GLchar* name, const OGL_TYPE* v, GLsizei count) const {           \
    glUniform4##TYPE_SUFFIX##v(this->GetUniformLocation(name), count, v);     \
  }

__ALPHA_SHADER_PROGRAM_UNIFORM_IMPLEMENTATION(GLfloat, f)

__ALPHA_SHADER_PROGRAM_UNIFORM_IMPLEMENTATION(GLdouble, d)

__ALPHA_SHADER_PROGRAM_UNIFORM_IMPLEMENTATION(GLint, i)

__ALPHA_SHADER_PROGRAM_UNIFORM_IMPLEMENTATION(GLuint, ui)

// undef macro
#undef __ALPHA_SHADER_PROGRAM_UNIFORM_IMPLEMENTATION

std::string ShaderProgram::LoadShaderSource(const std::string& fileName) {
  try {
    std::ifstream fparser;
    fparser.open(fileName, std::ios_base::in);
    if (fparser) {
      std::string strBuffer(std::istreambuf_iterator<char>(fparser),
                            (std::istreambuf_iterator<char>()));
      return strBuffer;
    } else
      throw IOException(
          std::string("Could not open file \"").append(fileName).append("\""));
  } catch (const std::exception& e) {
    BOOST_LOG_TRIVIAL(error) << e.what();
    assert(1);
  }
  return "";
}
void ShaderProgram::Dispose() {
  if (!m_hasDisposed) {
    glDeleteProgram(m_object);
    m_object = -1;
    m_hasDisposed = true;
  }
}
}
