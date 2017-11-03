#include "../include/Physics/PhysicsEngine.h"
#include "../include/Physics/BoundingSphere.h"

using namespace gpx;

void PhysicsEngine::AddObject(const PhysicsObject& object){
	m_objects.push_back(object);
}

void PhysicsEngine::Simulate(float delta){
	for(auto it = std::begin(m_objects); it != std::end(m_objects); ++it)
		it->Integrate(delta);
}

void PhysicsEngine::HandleCollisions(){
	for(unsigned i = 0; i < m_objects.size(); ++i)
	{
		for(unsigned j = i + 1; j < m_objects.size(); ++j)
		{
			IntersectData intersectData =
					m_objects[i].GetBoundingSphere().Intersect(m_objects[j].GetBoundingSphere());
			if(intersectData.Intersects()){
				m_objects[i].SetVelocity(m_objects[i].GetVelocity() * -1.0f);
				m_objects[j].SetVelocity(m_objects[i].GetVelocity() * -1.0f);
			}
		}
	}
}
