#pragma once

#include "Transform.h"

class Light
{
public: 
	Light();
	virtual ~Light();


public:
	Transform transform;

	glm::vec3 lightColor; 
	float lightIntensity;

	float ambientStr;
	glm::vec3 ambientColor;
	float specStr;
	float specPhong;
};

