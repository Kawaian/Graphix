#pragma once

#include "Entity.h"
#include "../include/Graphics/Material.h"
#include "../include/Graphics/Mesh.h"

namespace gpx
{


	class Spatial : public Entity
	{
	public:
		Spatial(
			const CMesh* mesh,
			const glm::vec3& position = glm::vec3(0.0, 0.0, 0.0),
			const glm::vec3& rotation = glm::vec3(0.0, 0.0, 0.0),
			const glm::vec3& scale = glm::vec3(1.0, 1.0, 1.0)

			)
			:
			Entity(position, rotation, scale),
			m_mesh(mesh),
			m_material(CreateTexture("assets/images/White.png"))
		{}
		virtual ~Spatial(){}

		virtual void Render(const ShaderSmtPtr& shader){
			m_material.Bind(shader);
			m_mesh->Render();
		}

		virtual void Render(const ShaderProgram* shader) {
			m_material.Bind(shader);
			m_mesh->Render();
		}

		const CMesh *GetMesh() const { return m_mesh; }
		void SetMesh(const CMesh *mesh) { m_mesh = mesh; }
		Material GetMaterial() const { return m_material; }
		void SetMaterial(const Material& material) { m_material = material; }
	protected:
		const CMesh *m_mesh;
		Material m_material;
	};
}