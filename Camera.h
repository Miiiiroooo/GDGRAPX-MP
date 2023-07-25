#pragma once

#include "Transform.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetCamTarget(glm::vec3 camTarget);
	glm::vec3 GetCamTarget();
	glm::mat4 GetProjectionMatrix();

protected:
	virtual void ComputeProjectionMatrix() = 0;


private:
	glm::vec3 camTarget;

protected:
	glm::mat4 projectionMatrix;

public:
	Transform transform;
};

