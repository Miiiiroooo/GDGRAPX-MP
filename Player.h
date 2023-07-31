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
	void CheckInputs(float deltaTime, Camera* mainCamera);

private:
	void OnMouseInputs(float deltaTime, Camera* mainCamera);
	void OnMovementInputs(float deltaTime, Camera* mainCamera);
	void OnBinocularInputs(float deltaTime, Camera* mainCamera);
	void OnOrthoCameraInputs(float deltaTime, Camera* mainCamera);


public:
	Transform transform;
	bool isInThirdPerson = true;

private:
	// Controls/Inputs
	Dictionary<int, bool> heldKeyInputs;
	Dictionary<int, bool> heldButtonInputs;
	const float movementSpeed = 5.f;
	const float rotationSpeed = 45.f;
	const float zoomSpeed = 10.f;
	LightIntensity intensity = LightIntensity::Low;
	Cameras currentCamera = Cameras::Third_Person;

	// Mouse
	const float mouseSensitivity = 100.f;
	glm::vec2 oldCursorPos = glm::vec2(0.f);
	glm::vec2 currentCursorPos = glm::vec2(0.f);
	glm::vec2 mouseDir = glm::vec2(0.f);

	// Other properties
	Model3D* model;
	Light* light;
};

