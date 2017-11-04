#include <vector>
#include <glm/glm.hpp>
#include "../include/Graphics/LogManager.h"
#include "../include/Graphics/Mesh.h"
#include <boost/log/trivial.hpp>

namespace gpx {

    MeshSmtPtr CreateMesh(
        Vertex* vertices, unsigned int numVertices,
        unsigned int* indeces, unsigned int numIndices
    )
    {
        return std::make_shared<CMesh>(vertices, numVertices, indeces, numIndices);
    }
    MeshSmtPtr CreateMesh(const char* fileName) {
        return std::make_shared<CMesh>(fileName);
    }
    // ----------------------------------------------

    CMesh::CMesh(Vertex* vertices, unsigned int numVertices,
        unsigned int* indeces, unsigned int numIndeces) {
        Geometry model;
        for (unsigned int i = 0; i < numVertices; i++) {
            model.positions.push_back(*vertices[i].getPos());
            model.texCoords.push_back(*vertices[i].getTexCoord());
            model.normals.push_back(*vertices[i].getNormal());
        }
        for (unsigned int i = 0; i < numIndeces; i++)
            model.indices.push_back(indeces[i]);
        __InitMesh(model);
    }

    CMesh::CMesh(const char* fileName) {

        const aiScene* scene = importer_.ReadFile(fileName,
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs |
            aiProcess_CalcTangentSpace
        );
        if (scene == nullptr) {
            BOOST_LOG_TRIVIAL(error) << "Couldn't Load Mesh from file : " << fileName;
            assert(false);
        }

        // Posprocess scene
        scene = importer_.ApplyPostProcessing(aiProcess_CalcTangentSpace);


        const aiMesh* model = scene->mMeshes[0];

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        const aiVector3D aiZeroVector(.0f, .0f, .0f);
        Geometry out;

        for (unsigned i = 0; i < model->mNumVertices; ++i)
        {
            const aiVector3D* pPos = &(model->mVertices[i]);
            const aiVector3D* pNormal = &(model->mNormals[i]);
            const aiVector3D* pTexCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][i]) : &aiZeroVector;
            const aiVector3D* pTangent = &(model->mTangents[i]);


            Vertex vert(
                glm::vec3(pPos->x, pPos->y, pPos->z),		       	    //positions
                glm::vec2(pTexCoord->x, pTexCoord->y),              //UV coords
                glm::vec3(pNormal->x, pNormal->y, pNormal->z),      //normals
                glm::vec3(pTangent->x, pTangent->y, pTangent->z)    //tangents

            );

            vertices.push_back(vert);

            out.positions.push_back(*vert.getPos());
            out.texCoords.push_back(*vert.getTexCoord());
            out.normals.push_back(*vert.getNormal());
            out.tangents.push_back(*vert.getTangent());
        }
        for (unsigned i = 0; i < model->mNumFaces; ++i) {
            const aiFace& face = model->mFaces[i];
            assert(face.mNumIndices == 3);
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }

        out.indices = indices;
        __InitMesh(out);
    }

    CMesh::CMesh(CMesh&& other) {
        this->m_vertexArrayObject = other.m_vertexArrayObject;
        other.m_vertexArrayObject = 0;
        for (unsigned i = 0; i < NUM_BUFFERS; ++i) {
            this->m_vertexArrayBuffers[i] = other.m_vertexArrayBuffers[i];
            other.m_vertexArrayBuffers[i] = 0;
        }
        this->m_drawCount = other.m_drawCount;
        this->m_hasDisposed = other.m_hasDisposed;
        other.m_hasDisposed = true;
    }

    // ----------------------------------------------

    void CMesh::__InitMesh(const Geometry& model) {
        m_drawCount = model.indices.size();

        glGenVertexArrays(1, &m_vertexArrayObject);
        glBindVertexArray(m_vertexArrayObject);
        glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

        BOOST_LOG_TRIVIAL(trace) << "Building Mesh at location h" << std::hex << m_vertexArrayObject << std::dec;

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
        glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
        glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
        glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TANGENT_VB]);
        glBufferData(GL_ARRAY_BUFFER, model.tangents.size() * sizeof(model.tangents[0]), &model.tangents[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);

        glBindVertexArray(0);
    }

    void CMesh::PushData(const Geometry& model) {
        m_drawCount = model.indices.size();
        glBindVertexArray(m_vertexArrayObject);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
        glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
        glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
        glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TANGENT_VB]);
        glBufferData(GL_ARRAY_BUFFER, model.tangents.size() * sizeof(model.tangents[0]), &model.tangents[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    CMesh::~CMesh() {
        this->Dispose();
    }

    void CMesh::Render(GLenum drawmode) const {
        glBindVertexArray(m_vertexArrayObject);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glDrawElements(drawmode, m_drawCount, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);


        glBindVertexArray(0);
    }

    bool CMesh::HasDisposed() {
        return m_hasDisposed;
    }

    void CMesh::Dispose() {
        if (!m_hasDisposed) {
            glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
            glDeleteVertexArrays(1, &m_vertexArrayObject);
            m_hasDisposed = true;
        }
    }
}
