#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "../include/Graphics/Shader.h"
#include "../include/Graphics/Mesh.h"
#include "../include/Graphics/Texture.h"
#include "../include/Core/Types.h"

namespace gpx {
struct AssetLoader {
 public:
  using ShaderRepo =
      std::unordered_map<std::string, std::unique_ptr<ShaderProgram>>;
  using MeshRepo = std::unordered_map<std::string, std::unique_ptr<CMesh>>;
  using TextureRepo =
      std::unordered_map<std::string, std::unique_ptr<CTexture>>;

 
  GLOBAL std::unique_ptr<ShaderProgram>& LoadShader(const std::string& ID, const char* vs,
                                         const char* fs) {
    shaderObjects.insert(std::pair<std::string, std::unique_ptr<ShaderProgram>>(
        ID, std::make_unique<ShaderProgram>(
                (shaderRootDir + std::string(vs)).c_str(),
                (shaderRootDir + std::string(fs)).c_str())));    
    return (shaderObjects.at(ID));
  }

  GLOBAL std::unique_ptr<ShaderProgram>& LoadShader(const std::string& ID, const char* vs,
                                         const char* fs, const char* gs) {
    shaderObjects.insert(std::pair<std::string, std::unique_ptr<ShaderProgram>>(
        ID, std::make_unique<ShaderProgram>(
                (shaderRootDir + std::string(vs)).c_str(),
                (shaderRootDir + std::string(fs)).c_str(),
                (shaderRootDir + std::string(gs)).c_str())));
    return (shaderObjects.at(ID));
  }

  GLOBAL std::unique_ptr<ShaderProgram>& Shader(const std::string& ID) {
    return (shaderObjects.at(ID));
  }

  // Mesh
  GLOBAL std::unique_ptr<CMesh>& LoadMesh(const std::string& ID, Vertex* vertices,
                         unsigned int numVertices, unsigned int* indeces,
                         unsigned int numIndices) {
    meshObjects.insert(std::pair<std::string, std::unique_ptr<CMesh>>(
        ID,
        std::make_unique<CMesh>(vertices, numVertices, indeces, numIndices)));
    return (meshObjects.at(ID));
  }

  GLOBAL std::unique_ptr<CMesh>& LoadMesh(const std::string& ID, const char* fileName) {
    meshObjects.insert(std::pair<std::string, std::unique_ptr<CMesh>>(
        ID, std::make_unique<CMesh>(
                (meshRootDir + std::string(fileName)).c_str())));
    return (meshObjects.at(ID));
  }

  GLOBAL std::unique_ptr<CMesh>& Mesh(const std::string& ID) {
    return (meshObjects.at(ID));
  }

  // Texture
  GLOBAL std::unique_ptr<CTexture>& LoadTexture(
      const std::string& ID, const char* texturePath,
      TextureType texTarget = TextureType::Texture2D,
      GLfloat filter = GL_LINEAR, GLfloat pattern = GL_REPEAT,
      GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA,
      GLenum attachment = GL_NONE) {
    textureObjects.insert(std::pair<std::string, std::unique_ptr<CTexture>>(
        ID, std::unique_ptr<CTexture>(new CTexture(
                (textureRootDir + std::string(texturePath)).c_str(), texTarget,
                filter, pattern, internalFormat, format, attachment))));
    return (textureObjects.at(ID));
  }

  GLOBAL std::unique_ptr<CTexture>& LoadTexture(
      const std::string& ID, int width, int height,
      unsigned char* data = nullptr,
      TextureType texTarget = TextureType::Texture2D,
      GLfloat filter = GL_LINEAR, GLfloat pattern = GL_REPEAT,
      GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA,
      GLenum attachment = GL_NONE) {
    textureObjects.insert(std::pair<std::string, std::unique_ptr<CTexture>>(
        ID, std::unique_ptr<CTexture>(
                new CTexture(width, height, data, texTarget, filter, pattern,
                             internalFormat, format, attachment))));
    return (textureObjects.at(ID));
  }

  GLOBAL std::unique_ptr<CTexture>& LoadTextureCustomPath(
      const std::string& ID, const char* texturePath,
      TextureType texTarget = TextureType::Texture2D,
      GLfloat filter = GL_LINEAR, GLfloat pattern = GL_REPEAT,
      GLenum attachment = GL_NONE) {
    textureObjects.insert(std::pair<std::string, std::unique_ptr<CTexture>>(
        ID, std::unique_ptr<CTexture>(new CTexture(
                texturePath, texTarget, filter, pattern, attachment))));
    return (textureObjects.at(ID));
  }

  GLOBAL std::unique_ptr<CTexture>& Texture(const std::string& ID) {
    return (textureObjects.at(ID));
  }
 private:
  LOCAL_PERSIST ShaderRepo shaderObjects;
  LOCAL_PERSIST MeshRepo meshObjects;
  LOCAL_PERSIST TextureRepo textureObjects;
  LOCAL_PERSIST std::string shaderRootDir;
  LOCAL_PERSIST std::string meshRootDir;
  LOCAL_PERSIST std::string textureRootDir;
};
}
