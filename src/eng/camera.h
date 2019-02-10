#pragma once

#include <glm/glm.hpp>

// CTauCamera
// Holds projection and view matrix.
// Calculates only perspective matrix.
class CTauCamera
{
public:
	CTauCamera(float x, float y, float z);
	~CTauCamera(void);

	void Recalculate(void);
	void Move       (float dx, float dy, float dz);
	void SetPosition(float x,  float y,  float z);
	void ForceLook  (float x, float y, float z);
	void Forward    (float amount);
	void Sidestep   (float amount);
	void Turn       (float amount);
	
	inline void ResetPosition(void)             { m_position = m_initialPosition; };

	glm::vec2       GetForwardVector(float forward_step);
	const float*    GetValueProjection(void) const { return glm::value_ptr(m_projection); }
	const float*    GetValueView(void)       const { return glm::value_ptr(m_view);       }
	const glm::vec3 GetPosition(void)        const { return m_position;                   }

private:
	const glm::vec3 m_initialPosition;
	glm::vec3 m_position, m_right;
	glm::mat4 m_projection, m_view;
	float m_angle;
};

