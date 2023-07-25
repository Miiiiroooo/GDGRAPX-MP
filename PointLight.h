#pragma once

#include "Light.h"
#include "Model3D.h"

class PointLight : public Light 
{
public:
	PointLight();
	~PointLight();

	void ChangeColor(glm::vec3 newColor);


public:
	float radius;
	Model3D* model;
};

