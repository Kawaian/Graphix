#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace gpx
{


	class Transform{
	public:

		Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
			: _pos(pos), _rot(rot), _scale(scale){}

		glm::mat4 GetModel() const{
			glm::mat4 mat_pos = glm::translate(_pos);
			glm::mat4 mat_rotX = glm::rotate(_rot.x, glm::vec3(1, 0, 0));
			glm::mat4 mat_rotY = glm::rotate(_rot.y, glm::vec3(0, 1, 0));
			glm::mat4 mat_rotZ = glm::rotate(_rot.z, glm::vec3(0, 0, 1));
			glm::mat4 mat_scale = glm::scale(_scale);
			glm::mat4 mat_rot = mat_rotZ * mat_rotY * mat_rotX;
			return mat_pos * mat_rot * mat_scale;
		}

		inline glm::vec3 GetPos() const{ return _pos; }
		inline glm::vec3 GetRot() const{ return _rot; }
		inline glm::vec3 GetScale() const{ return _scale; }
		inline void SetPos(const glm::vec3& pos){ _pos = pos; }
		inline void SetRot(const glm::vec3& rot){ _rot = rot; }
		inline void SetScale(const glm::vec3& scale){ _scale = scale; }

		inline void OffSetPos(const glm::vec3& pos){ _pos += pos; }
		inline void OffSetRot(const glm::vec3& rot){ _rot += rot; }
		inline void OffSetScale(const glm::vec3& scale){ _scale += scale; }

		inline void Concatenate(const Transform& other){
			_pos += other._pos;
			_rot += other._rot;
			_scale += other._scale;
		}

		inline void Reset(){
			_pos = glm::vec3(0.0, 0.0, 0.0);
			_rot = glm::vec3(0.0, 0.0, 0.0);
			_scale = glm::vec3(0.0, 0.0, 0.0);
		}

	private:
		glm::vec3  _pos;
		glm::vec3  _rot;
		glm::vec3  _scale;
	};

}
