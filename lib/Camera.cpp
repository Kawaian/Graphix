#include <glm/gtx/transform.hpp>
#include <assert.h>
#include "../include/Graphics/Camera.h"
#include "../include/Graphics/LogManager.h"
#include <cmath>

static const float PREV_GIMBLE_LOCK = 85.9999f;

using namespace gpx;

Camera::Camera() :
    m_position(.0f, .0f, .0f),
    m_horizAngle(.0f),
    m_vertAngle(.0f),
    m_fov(70.0f),
    m_near(0.00001f),
    m_far(100.0f),
    m_viewPort(1.33) {} ///default values

Camera::Camera(glm::vec3 pos, float fieldOfView, float aspect, float nearPlane, float farPlane, float horizontleAngle, float verticalAngle) :
    m_position(pos),
    m_horizAngle(horizontleAngle),
    m_vertAngle(verticalAngle),
    m_fov(fieldOfView),
    m_near(nearPlane),
    m_far(farPlane),
    m_viewPort(aspect) {} ///user values

void Camera::SetUniforms(const ShaderSmtPtr& shader, Transform transform) const{
    glm::mat4 model = transform.GetModel();
    shader->UpdateUniformMatrix4f("MVP", this->Combined() * model);
    shader->UpdateUniformMatrix4f("model", model);
    shader->UpdateUniformMatrix3f("normalMatrix",
        glm::inverseTranspose(glm::mat3(model))
    );
    shader->UpdateUniform3fv("cameraPosition", glm::value_ptr(this->m_position));
}


void Camera::SetUniforms(const ShaderProgram* shader, Transform transform) const {
	glm::mat4 model = transform.GetModel();
	shader->UpdateUniformMatrix4f("MVP", this->Combined() * model);
	shader->UpdateUniformMatrix4f("model", model);
	shader->UpdateUniformMatrix3f("normalMatrix",
		glm::inverseTranspose(glm::mat3(model))
		);
	shader->UpdateUniform3fv("cameraPosition", glm::value_ptr(this->m_position));
}


void Camera::SetFieldOfView(float fov)
{
    assert(fov > 0.0f && fov < 180.0f);
    m_fov = fov;
}

void Camera::SetNearPlane(float near)
{
    assert(near < .0f);
    m_near = near;
}
void Camera::SetFarPlane(float far)
{
    assert(far > m_near);
    m_far = far;
}

void Camera::SetViewPortAscpectRatio(float aspect){
    assert(aspect > 0.0);
    m_viewPort = aspect;
}

glm::mat4 Camera::GetOrientation() const
{
    glm::mat4 ort;
    ort = glm::rotate(ort, glm::radians(m_vertAngle), glm::vec3(1.0, .0, .0) );
    ort = glm::rotate(ort, glm::radians(m_horizAngle), glm::vec3(.0, 1.0, .0));
    return ort;
}

void Camera::OffSetOrientation(float up, float right){
    m_horizAngle += right;
    m_vertAngle += up;
    this->Normalize();
}

void Camera::LookAt(glm::vec3 pos)
{
    assert(pos != m_position);
    glm::vec3 dir = glm::normalize(pos - m_position);
    m_vertAngle = glm::radians(asinf(-dir.y));
    m_horizAngle = -glm::radians(atan2f(-dir.x, -dir.z));
    Normalize();
}

glm::vec3 Camera::GetForward() const{
    glm::vec4 f = glm::inverse(GetOrientation()) * glm::vec4(.0, .0, -1.0, 1.0);
    return glm::vec3(f);
}

glm::vec3 Camera::GetRight() const{
    glm::vec4 r = glm::inverse(GetOrientation()) * glm::vec4(1.0, .0, .0, 1.0);
    return glm::vec3(r);
}

glm::vec3 Camera::GetUp() const{
    glm::vec4 u = glm::inverse(GetOrientation()) * glm::vec4(.0, 1.0, .0, 1.0);
    return glm::vec3(u);
}

glm::mat4 Camera::Combined() const{
    return Projection() * View();
}

glm::mat4 Camera::Projection() const{
    return glm::perspective(m_fov, m_viewPort, m_near, m_far);
	//return glm::ortho(0.0, -1000.0, 0.0, -900.0 );
}

glm::mat4 Camera::View() const{
    return GetOrientation() * glm::translate(glm::mat4(), -m_position);
}

void Camera::Normalize() {
    m_horizAngle = fmodf(m_horizAngle, 360.0f);
    if(m_horizAngle < .0f)
        m_horizAngle += 360.0f;/// make positive
    if(m_vertAngle > PREV_GIMBLE_LOCK)
        m_vertAngle = PREV_GIMBLE_LOCK;
    else if(m_vertAngle < -PREV_GIMBLE_LOCK)
        m_vertAngle = -PREV_GIMBLE_LOCK;
}
