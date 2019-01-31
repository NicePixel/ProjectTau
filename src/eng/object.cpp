#include "object.h"

CTauObject::CTauObject(float x, float y, float z, int uid, MESH* mesh):
	m_uid(uid)
{
	m_position = glm::vec3(x, y, z);
	m_mesh     = mesh;
	RecalculateMatrices();
}

CTauObject::~CTauObject(void)
{

}

const float* CTauObject::GetValueMat4Position(void)
{
	return glm::value_ptr(m_model);
}

void CTauObject::RecalculateMatrices(void)
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, m_position);
}