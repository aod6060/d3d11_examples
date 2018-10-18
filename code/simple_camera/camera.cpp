#include "sys.h"

void Camera::init(
	const glm::vec3& pos,
	const glm::vec2& rot,
	float fov,
	float aspect,
	float minZ,
	float maxZ)
{
	this->pos = pos;
	this->rot = rot;
	this->fov = fov;
	this->aspect = aspect;
	this->minZ = minZ;
	this->maxZ = maxZ;
}

void Camera::update(float delta)
{
	int32_t mx = 0;
	int32_t my = 0;

	input_getPosition(mx, my);

	rot.y += this->rotSpeed * mx * ((delta - 0.001f < 0) ? 0.001f : delta);
	rot.x += this->rotSpeed * my * ((delta - 0.001f < 0) ? 0.001f : delta);

	if (rot.x < -90.0f)
	{
		rot.x = -90.0f;
	}

	if (rot.x > 90.0f)
	{
		rot.x = 90.0f;
	}

	if (rot.y <= -360.0f)
	{
		rot.y += 360.0f;
	}

	if (rot.y >= 360.0f)
	{
		rot.y -= 360.0f;
	}


	float yrad = glm::radians(rot.y);
	float sp = this->walkSpeed;

	if (input_isKeyPress(Keyboard::KEY_E))
	{
		sp *= 3.0f;
	}

	if (input_isKeyPress(Keyboard::KEY_W))
	{
		pos.x += sp * glm::sin(yrad) * delta;
		pos.z -= sp * glm::cos(yrad) * delta;
	}

	if (input_isKeyPress(Keyboard::KEY_S))
	{
		pos.x -= sp * glm::sin(yrad) * delta;
		pos.z += sp * glm::cos(yrad) * delta;
	}

	if (input_isKeyPress(Keyboard::KEY_A))
	{
		pos.x -= sp * glm::cos(yrad) * delta;
		pos.z -= sp * glm::sin(yrad) * delta;
	}

	if (input_isKeyPress(Keyboard::KEY_D))
	{
		pos.x += sp * glm::cos(yrad) * delta;
		pos.z += sp * glm::sin(yrad) * delta;
	}

	if (input_isKeyPress(Keyboard::KEY_LSHIFT))
	{
		pos.y -= sp * delta;
	}

	if (input_isKeyPress(Keyboard::KEY_SPACE))
	{
		pos.y += sp * delta;
	}
}

glm::mat4 Camera::toProj()
{
	return glm::perspective(
		this->fov,
		this->aspect,
		this->minZ,
		this->maxZ
	);
}

glm::mat4 Camera::toView()
{
	glm::mat4 rX = glm::rotate(glm::mat4(1.0f), glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rY = glm::rotate(glm::mat4(1.0f), glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 t = glm::translate(glm::mat4(1.0f), -this->pos);

	return rX * rY * t;
}