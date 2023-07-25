#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	~DirectionalLight();


public:
	glm::vec3 lightDir;
};

