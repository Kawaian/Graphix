#pragma once

namespace gpx
{


	class IntersectData
	{
	public:
		IntersectData(const bool intersect, const float distance)
			:
			m_intersect(intersect),
			m_distance(distance)
		{}

		bool Intersects() const { return m_intersect; }
		float GetDistance() const { return m_distance; }
	private:
		const bool m_intersect;
		const float m_distance;
	};
}