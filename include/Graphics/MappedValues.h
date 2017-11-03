#pragma once

#include <map>
#include <glm/glm.hpp>

namespace gpx
{

	class MappedValues
	{
	public:
		inline void AddVec3(const char* name, glm::vec3 value){ _vec3Map.insert(std::pair<const char*, glm::vec3>(name, value)); }
		inline void AddFloat(const char* name, float value){ _floatMap.insert(std::pair<const char*, float>(name, value)); }

		inline glm::vec3 GetVec3(const char* name) const{
			std::map<const char*, glm::vec3>::const_iterator it = _vec3Map.find(name);
			if (it != _vec3Map.end())
				return it->second;
			return glm::vec3(0, 0, 0);
		}

		inline float GetFloat(const char* name) const{
			std::map<const char*, float>::const_iterator it = _floatMap.find(name);
			if (it != _floatMap.end())
				return it->second;
			return it->second;
		}
	private:
		std::map<const char*, glm::vec3> _vec3Map;
		std::map<const char*, float> _floatMap;
	};
}