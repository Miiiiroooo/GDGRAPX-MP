#include "pch.h"
#include "Player.h"
#include "PerspectiveCamera.h"
#include "OrthoCamera.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::SetPlayerModel(Model3D* model)
{
    this->model = model;
}

void Player::SetLight(Light* light)
{
    this->light = light;
}

Cameras Player::GetCurrentCamera()
{
    return currentCamera;
}

void Player::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // Press inputs
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        intensity = (int)intensity + 1 > 2 ? (LightIntensity)0 : (LightIntensity)(intensity + 1);
        light->lightIntensity = 1.4f * (int)(intensity + 1);
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        currentCamera = isInThirdPerson ? Cameras::First_Person : Cameras::Third_Person;
        isInThirdPerson = currentCamera == Cameras::Third_Person; 
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        currentCamera = Cameras::Birds_Eye;
    }


    // Hold inputs
    if (action == GLFW_PRESS && (
        key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_D || key == GLFW_KEY_A || 
        key == GLFW_KEY_Q || key == GLFW_KEY_E))
    {
        heldKeyInputs[key] = true;
    }
    else if (action == GLFW_RELEASE && ( 
        key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_D || key == GLFW_KEY_A || 
        key == GLFW_KEY_Q || key == GLFW_KEY_E)) 
    {
        heldKeyInputs[key] = false;
    }
}

void Player::CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
    // Get the change in position of the cursor and calculate its direction
    oldCursorPos = currentCursorPos;
    currentCursorPos = glm::vec2(xPos, yPos);

    mouseDir = currentCursorPos - oldCursorPos;
    mouseDir = glm::normalize(mouseDir);
}

void Player::MouseButtonCallback(GLFWwindow* window, int button, int action, int mod)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
    {
        heldButtonInputs[GLFW_MOUSE_BUTTON_1] = true;
    }

    if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_1)
    {
        heldButtonInputs[GLFW_MOUSE_BUTTON_1] = false;
    }
}

void Player::CheckInputs(float deltaTime)
{
    OnMovementInputs(deltaTime);
    OnBinocularInputs(deltaTime);
    OnOrthoCameraInputs(deltaTime);
}

void Player::OnMovementInputs(float deltaTime)
{
    if (currentCamera != Cameras::Third_Person)
        return;

    glm::vec3 deltaPos = glm::vec3(0.f);

    if (heldKeyInputs[GLFW_KEY_W])
    {
        deltaPos += transform.localForward * movementSpeed * deltaTime;
    }

    if (heldKeyInputs[GLFW_KEY_S])
    {
        deltaPos -= transform.localForward * movementSpeed * deltaTime;
    }

    glm::vec3 deltaRot(0.f);

    if (heldKeyInputs[GLFW_KEY_A])
    {
        deltaRot.y -= rotationSpeed * deltaTime;
    }

    if (heldKeyInputs[GLFW_KEY_D])
    {
        deltaRot.y += rotationSpeed * deltaTime;
    }

    transform.position += deltaPos;
    model->transform.position += deltaPos;
    light->transform.position += deltaPos;

    transform.Rotate(deltaRot);
    model->transform.Rotate(deltaRot);
    light->transform.Rotate(deltaRot); 
}

void Player::OnBinocularInputs(float deltaTime)
{
    if (currentCamera != Cameras::First_Person)
        return;

    if (heldKeyInputs[GLFW_KEY_W])
    {

    }

    if (heldKeyInputs[GLFW_KEY_S])
    {

    }

    if (heldKeyInputs[GLFW_KEY_A])
    {

    }

    if (heldKeyInputs[GLFW_KEY_D])
    {

    }

    if (heldKeyInputs[GLFW_KEY_Q])
    {

    }

    if (heldKeyInputs[GLFW_KEY_E])
    {

    }
}

void Player::OnOrthoCameraInputs(float deltaTime)
{
    if (currentCamera != Cameras::Birds_Eye)
        return;

    if (heldKeyInputs[GLFW_KEY_W])
    {

    }

    if (heldKeyInputs[GLFW_KEY_S])
    {

    }

    if (heldKeyInputs[GLFW_KEY_A])
    {

    }

    if (heldKeyInputs[GLFW_KEY_D])
    {

    }
}
