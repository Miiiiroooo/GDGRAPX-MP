#include "pch.h"
#include "PointLight.h"

PointLight::PointLight()
{
	radius = 10.f;
}

PointLight::~PointLight()
{

}

void PointLight::ChangeColor(glm::vec3 newColor)
{
	lightColor = newColor;
	model->color = newColor;
}
