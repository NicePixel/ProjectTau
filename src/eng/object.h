/* object.h
 * Object contains data about an entity in the world.
 *
 * It has (x, y, z) coordiante, a unique identificator and optionally a MESH
 * used for rendering.
 *
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "r_mesh.h"

class CTauObject
{
public:
	CTauObject(float x, float y, float z, int uid, MESH* mesh);
	~CTauObject(void);

	inline glm::vec3    GetPosition(void)          const { return m_position; }
	inline int          GetUID(void)               const { return m_uid;      }
	inline MESH*        GetMesh(void)                    { return m_mesh;     }
	const float*        GetValueMat4Position(void);

private:
	const int m_uid;
	glm::vec3 m_position;
	glm::mat4 m_model;
	MESH* m_mesh;

	void RecalculateMatrices(void);
};
