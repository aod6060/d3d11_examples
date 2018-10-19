#pragma once


struct Camera
{
	glm::vec3 pos;
	glm::vec2 rot;

	float fov = 0.0f;
	float aspect = 0.0f;
	float minZ = 1.0f;
	float maxZ = 1024.0f;

	float rotSpeed = 128.0f;
	float walkSpeed = 32.0f;

	void init(
		const glm::vec3& pos, 
		const glm::vec2& rot,
		float fov,
		float aspect,
		float minZ,
		float maxZ);

	void update(float delta);

	glm::mat4 toProj();
	
	glm::mat4 toView();

};