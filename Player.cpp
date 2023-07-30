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
        if (currentCamera == Cameras::Birds_Eye && isInThirdPerson)
        {
            currentCamera = Cameras::Third_Person;
        }
        else if (currentCamera == Cameras::Birds_Eye && !isInThirdPerson)
        {
            currentCamera = Cameras::First_Person;
        }
        else
        {
            currentCamera = isInThirdPerson ? Cameras::First_Person : Cameras::Third_Person;
            isInThirdPerson = currentCamera == Cameras::Third_Person;
        }
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

void Player::CheckInputs(float deltaTime, Camera* mainCamera)
{
    OnMovementInputs(deltaTime, mainCamera);
    OnBinocularInputs(deltaTime, mainCamera);
    OnOrthoCameraInputs(deltaTime, mainCamera);
}

void Player::OnMovementInputs(float deltaTime, Camera* mainCamera)
{
    if (currentCamera != Cameras::Third_Person)
        return;

    glm::vec3 deltaPos(0.f);

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
    mainCamera->transform.position += deltaPos;

    transform.Rotate(deltaRot);
    model->transform.Rotate(deltaRot);
    light->transform.RotateAroundPoint(deltaRot, transform.position); 
    mainCamera->transform.RotateAroundPoint(deltaRot, transform.position);
}

void Player::OnBinocularInputs(float deltaTime, Camera* mainCamera)
{
    if (currentCamera != Cameras::First_Person)
        return;

    PerspectiveCamera* firstPersonCam = ConvertTo<Camera, PerspectiveCamera>(mainCamera);

    glm::vec3 rotation = firstPersonCam->transform.GetEulerRotation();
    float deltaAngle = 0.f;
    float fov = firstPersonCam->fov;

    // converting from quat to euler angles usually result to different angles from what is expected
    // pitch and roll rotation are clamped between [-180, 180] and yaw rotation is clamped between [-90, 90]
    // since camera rotation never uses roll, we can use it as basis to get our expected euler angles
    bool checkRollNoValue = rotation.z <= 1.f && rotation.z >= -1.f; 


    if (heldKeyInputs[GLFW_KEY_W])
    {
        deltaAngle -= rotationSpeed * deltaTime;

        if (checkRollNoValue && rotation.x + deltaAngle > -50.f ||
            rotation.x + deltaAngle > 130.f ||
            rotation.x + deltaAngle < -170.f)
        {
            firstPersonCam->transform.RotateAroundAxis(deltaAngle, firstPersonCam->transform.localRight);
        }
    }

    if (heldKeyInputs[GLFW_KEY_S])
    {
        deltaAngle += rotationSpeed * deltaTime;

        if (checkRollNoValue && rotation.x + deltaAngle < 10.f ||
            rotation.x + deltaAngle > 130.f ||
            rotation.x + deltaAngle < -170.f)
        {
            firstPersonCam->transform.RotateAroundAxis(deltaAngle, firstPersonCam->transform.localRight);
        }
    }

    if (heldKeyInputs[GLFW_KEY_A])
    {
        deltaAngle -= rotationSpeed * deltaTime;
        firstPersonCam->transform.RotateAroundAxisAndPoint(deltaAngle, worldUp, transform.position);
    }

    if (heldKeyInputs[GLFW_KEY_D])
    {
        deltaAngle += rotationSpeed * deltaTime;
        firstPersonCam->transform.RotateAroundAxisAndPoint(deltaAngle, worldUp, transform.position);
    }

    if (heldKeyInputs[GLFW_KEY_Q])
    {
        fov += zoomSpeed * deltaTime;
        if (fov > 115)
            fov = 115;

        firstPersonCam->fov = fov; 
        firstPersonCam->ComputeProjectionMatrix(); 
    }

    if (heldKeyInputs[GLFW_KEY_E])
    {
        fov -= zoomSpeed * deltaTime;
        if (fov < 5)
            fov = 5;

        firstPersonCam->fov = fov; 
        firstPersonCam->ComputeProjectionMatrix(); 
    }
}

void Player::OnOrthoCameraInputs(float deltaTime, Camera* mainCamera)
{
    if (currentCamera != Cameras::Birds_Eye)
        return;

    glm::vec3 deltaPos(0.f);

    if (heldKeyInputs[GLFW_KEY_W])
    {
        deltaPos += transform.localForward * movementSpeed * 2.5f * deltaTime;
    }

    if (heldKeyInputs[GLFW_KEY_S])
    {
        deltaPos += -transform.localForward * movementSpeed * 2.5f * deltaTime;
    }

    if (heldKeyInputs[GLFW_KEY_A])
    {
        deltaPos += -transform.localRight * movementSpeed * 2.5f * deltaTime;
    }

    if (heldKeyInputs[GLFW_KEY_D])
    {
        deltaPos += transform.localRight * movementSpeed * 2.5f * deltaTime;
    }
    
    mainCamera->transform.position += deltaPos;
}
