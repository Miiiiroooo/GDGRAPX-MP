#pragma once

#include "Transform.h"
#include "Model3D.h"
#include "Camera.h"
#include "Light.h"

class Player
{
public:
	Player();
	~Player();

	void SetPlayerModel(Model3D* model);
	void SetLight(Light* light);
	Cameras GetCurrentCamera();

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
	void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mod);
	void CheckInputs(float deltaTime);

private:
	void OnMovementInputs(float deltaTime);
	void OnBinocularInputs(float deltaTime);
	void OnOrthoCameraInputs(float deltaTime);


public:
	Transform transform; 

private:
	// Controls/Inputs
	Dictionary<int, bool> heldKeyInputs;
	Dictionary<int, bool> heldButtonInputs;
	const float movementSpeed = 1.f;
	const float rotationSpeed = 45.f;
	LightIntensity intensity = LightIntensity::Low;
	Cameras currentCamera = Cameras::Third_Person;
	bool isInThirdPerson = true;

	// Mouse
	const float mouseSensitivity = 600.f;
	glm::vec2 oldCursorPos = glm::vec2(0.f);
	glm::vec2 currentCursorPos = glm::vec2(0.f);
	glm::vec2 mouseDir = glm::vec2(0.f);

	// Other properties
	Model3D* model;
	Light* light;
};

