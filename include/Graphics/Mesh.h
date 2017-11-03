#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <memory>

#include "Vertex.h"

namespace gpx {
struct Geometry{
	std::vector<glm::vec3>    positions;
	std::vector<glm::vec2>    texCoords;
	std::vector<glm::vec3>    normals;
	std::vector<glm::vec3>    tangents;
	std::vector<unsigned int> indices;
};

class CMesh {
public:
	CMesh(Vertex* vertices, unsigned int vertexCount,
     unsigned int* indices, unsigned int indexCount);
	CMesh(const char* fileName);
	virtual ~CMesh();
	void Render(GLenum drawmode = GL_TRIANGLES) const;
	void Dispose();
	bool HasDisposed();

  inline Geometry GetModel() const { return m_indexedModel; }
  void PushData(const Geometry& model);

	CMesh(CMesh&& other);
	CMesh(const CMesh& other) = delete;
	CMesh& operator=(const CMesh& other) = delete;
protected:
	enum{
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		TANGENT_VB,
		INDEX_VB,
		NUM_BUFFERS = 5
	};
	void __InitMesh(const Geometry& model);
	GLuint m_vertexArrayObject = 0;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;
	bool m_hasDisposed = false;
	Geometry m_indexedModel;
  Assimp::Importer importer_;
 };

typedef std::shared_ptr<CMesh> MeshSmtPtr;

MeshSmtPtr CreateMesh(Vertex* vertices, unsigned int numVertices,
	unsigned int* indeces, unsigned int numIndices);
MeshSmtPtr CreateMesh(const char* fileName);
}