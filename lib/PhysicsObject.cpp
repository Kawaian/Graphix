#include "../include/Physics/PhysicsObject.h"

using namespace gpx;

void PhysicsObject::Integrate(float delta){ m_position += m_velocity * delta; }
