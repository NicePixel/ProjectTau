#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"

CTauCamera::CTauCamera(float x, float y, float z):
	m_initialPosition(glm::vec3(x, y, z))
{
	m_position   = m_initialPosition;
	m_projection = glm::perspective(3.1415f / 2.0f, 800.0f / 600.0f, 0.2f, 10000.0f);
	m_angle      = 0.0f;
	m_right      = glm::vec3(1.0, 0.0, 0.0);
	Recalculate();
}

CTauCamera::~CTauCamera(void)
{

}

void CTauCamera::Recalculate(void)
{
	const float pitch = 0.0f;
	const glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	
	glm::vec3 front;
	front.x = cos(m_angle) * cos(pitch);
	front.y = sin(pitch);
	front.z = sin(m_angle) * cos(pitch);
	front = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	m_right = glm::normalize(glm::cross(front, up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.

	m_view = glm::lookAt(m_position, m_position + front, up);
}

void CTauCamera::ForceLook(float x, float y, float z)
{
	glm::vec3 up     = { 0.0f, 1.0f, 0.0f };
	glm::vec3 eye    = { m_position.x, m_position.y, m_position.z };
	glm::vec3 center = { x, y, z };
	m_view = glm::lookAt(eye, center, up);
}

void CTauCamera::Move(float dx, float dy, float dz)
{
	m_position.x += dx;
	m_position.y += dy;
	m_position.z += dz;
	Recalculate();
}

void CTauCamera::SetPosition(float x, float y, float z)
{
	m_position = glm::vec3(x, y, z);
	Recalculate();
}

void CTauCamera::Forward(float amount)
{
	m_position.x += cos(m_angle) * amount;
	m_position.z += sin(m_angle) * amount;
}

void CTauCamera::Sidestep(float amount)
{
	m_position.x += cos(m_angle + 3.1415f/2.0f) * amount;
	m_position.z += sin(m_angle + 3.1415f / 2.0f) * amount;
}

void CTauCamera::Turn(float amount)
{
	m_angle += amount;
}
