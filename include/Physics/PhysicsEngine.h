#pragma once

#include <vector>
#include "PhysicsObject.h"

namespace gpx
{

	class PhysicsEngine
	{
	public:
		PhysicsEngine(){}
		void AddObject(const PhysicsObject& object);

		///TODO Temp code
		PhysicsObject GetObject(unsigned int index) { return m_objects[index]; }
		void SetObject(PhysicsObject obj, unsigned int index) { m_objects[index] = obj; }

		void HandleCollisions();

		unsigned GetObjectCount() const{ return m_objects.size(); }

		void Simulate(float delta);
	private:
		std::vector<PhysicsObject> m_objects;
	};
}
