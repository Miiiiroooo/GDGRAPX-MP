#include "pch.h"
#include "ModelReference.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "OrthoCamera.h"
#include "PerspectiveCamera.h"


#pragma region Global Variables
// Models, Textures, Lights, and Cameras
List<ModelReference*> modelReferencesList;
List<Model3D*> modelsList;
List<Texture*> texturesList;
List<Light*> lightsList;
List<Camera*> camerasList;
Camera* mainCamera;

// Controls
Dictionary<int, bool> heldKeyInputs;
Dictionary<int, bool> heldButtonInputs;
bool isControllingLight = false;
const float modelRotationSpeed = 90.0f;
const float lightRotationSpeed = 90.0f;
const float dirIntensitySpeed = 0.5f;
const float pointIntensitySpeed = 1.2f;

// Mouse
const float mouseSensitivity = 600.f;
glm::vec2 oldCursorPos = glm::vec2(0.f);
glm::vec2 currentCursorPos = glm::vec2(0.f);
glm::vec2 mouseDir = glm::vec2(0.f);

// Time
float timeSinceStart = 0.f;
float oldTimeSinceStart = 0.f;
float deltaTime = 0.f;

// Projection
float fov = 90.f;
const float width = 600;
const float height = 600;
#pragma endregion


#pragma region Method Declarations
void ModelRotationInputs();
void LightRotationInputs();
void IntensityInputs();
void PerspectiveCamRotationInputs();
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mod);

bool InitializeOpenGL(GLFWwindow** window);
String GetShaderData(String path);
GLuint InitializeShaderProgram(const char* vertexData, const char* fragmentData, GLuint& vertShader, GLuint& fragShader);

void LoadObjects(const List<String>& objPathsList);
void LoadTextures(const List<Pair<String, GLuint>>& textureInfoList);
void CreateModels(const List<Pair<int, int>>& objTextureMap, const List<glm::vec3>& modelsInfoList);
void CreateLights(const List<Pair<glm::vec3, unsigned int>>& lightsInfoList);
void CreateCameras(const List<glm::vec3>& camerasInfoList);

glm::mat4 CreateViewMatrix();
void ComputeVerticesWithShaders(GLuint& shaderProgram, glm::mat4& transformationMatrix, glm::mat4& viewMatrix);
void ComputeFragmentsWithShaders(GLuint& shaderProgram, Texture* texture, glm::vec3& color);
void PassLightingData(GLuint& shaderProgram);
#pragma endregion


int main(void)
{
    // Initialize program
    srand(time(0));
    GLFWwindow* window;
    if (!InitializeOpenGL(&window))
        return -1;

    // Load shaders 
    String vertS = GetShaderData("Shaders/sample.vert");
    const char* v = vertS.c_str();
    String fragS = GetShaderData("Shaders/sample.frag");
    const char* f = fragS.c_str();

    GLuint vertShader, fragShader;
    GLuint shaderProgram = InitializeShaderProgram(v, f, vertShader, fragShader);


    // Load 3d object references
    List<String> objPathsList = {
        "3D/amumu.obj",   // source commented below
        "3D/sphere.obj",
    };
    LoadObjects(objPathsList);


    // Load textures
    List<Pair<String, GLuint>> textureInfoList = {
        {"3D/amumu.png", GL_RGBA},
    };
    LoadTextures(textureInfoList);


    // Create new models
    List<Pair<int, int>> objTextureMap = {
        {0, 0},   // index to obj reference (amumu.obj), index to texture (amumu.png)
        {1, -1},
    };
    List<glm::vec3> modelsInfoList = {
        glm::vec3(0.f, 0.f, 0.f),         // position
        glm::vec3(0.f, 180.f, 0.f),       // rotation
        glm::vec3(0.01f, 0.01f, 0.01f),   // scale
        glm::vec3(1.f, 1.f, 1.f),         // color
        glm::vec3(2.1213f, 2.f, 2.1213f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(1.f, 1.f, 1.f),
        glm::vec3(1.f, 1.f, 1.f),
    };
    CreateModels(objTextureMap, modelsInfoList);


    // Create Lights
    List<Pair<glm::vec3, unsigned int>> lightsInfoList = {
        {glm::vec3(4, -11, 3), 0},              // position, light type
        {glm::vec3(2.1213f, 2.f, 2.1213f), 1},
    };
    CreateLights(lightsInfoList);


    // Create Cameras
    List<glm::vec3> camerasInfoList = {
        glm::vec3(0.f, 0.5f, -2.f),       // position
        glm::vec3(0.f, 0.f, 0.f),         // rotation
        glm::vec3(0.f, 0.f, 0.f),         // camera target
        glm::vec3(0.f, 6.f, 0.f),
        glm::vec3(89.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 0.f),
    };
    CreateCameras(camerasInfoList);


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute deltaTime
        timeSinceStart = glfwGetTime();
        deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;

        // Check inputs
        ModelRotationInputs();
        LightRotationInputs();
        IntensityInputs();
        PerspectiveCamRotationInputs();

        // Iterate through modelsList
        for (Model3D* model : modelsList)
        {
            glm::mat4 transformationMatrix = model->GetTransformationMatrix();
            glm::mat4 viewMatrix = CreateViewMatrix();

            ComputeVerticesWithShaders(shaderProgram, transformationMatrix, viewMatrix);
            ComputeFragmentsWithShaders(shaderProgram, model->texture, model->color);

            glUseProgram(shaderProgram);
            glBindVertexArray(model->objRef->GetVAO());
            glDrawArrays(GL_TRIANGLES, 0, model->objRef->GetFullVertexData().size() / 8);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (Model3D* model : modelsList)
    {
        model->objRef->DeleteBufferObjects();
    }
    glfwTerminate();

    return 0;
}


#pragma region Input Callbacks
void ModelRotationInputs()
{
    if (isControllingLight)
        return;

    Model3D* model = modelsList[0];
    float delta = deltaTime * modelRotationSpeed;

    if (heldKeyInputs[GLFW_KEY_W])
    {
        model->transform.Rotate(glm::vec3(delta, 0.f, 0.f));
    }

    if (heldKeyInputs[GLFW_KEY_S])
    {
        model->transform.Rotate(glm::vec3(-delta, 0.f, 0.f));
    }

    if (heldKeyInputs[GLFW_KEY_A])
    {
        model->transform.Rotate(glm::vec3(0.f, delta, 0.f));
    }

    if (heldKeyInputs[GLFW_KEY_D])
    {
        model->transform.Rotate(glm::vec3(0.f, -delta, 0.f));
    }

    if (heldKeyInputs[GLFW_KEY_Q])
    {
        model->transform.Rotate(glm::vec3(0.f, 0.f, delta));
    }

    if (heldKeyInputs[GLFW_KEY_E])
    {
        model->transform.Rotate(glm::vec3(0.f, 0.f, -delta));
    }
}

void LightRotationInputs()
{
    if (!isControllingLight)
        return;

    // Get Point Light
    PointLight* light = ConvertTo<Light, PointLight>(lightsList[1]);
    glm::vec3 delta(0.f);

    // Get model target and set as the anchor point for rotation
    Model3D* target = modelsList[0];
    glm::vec3 anchorPoint = target->transform.position;

    if (heldKeyInputs[GLFW_KEY_W])
    {
        delta.x -= deltaTime * lightRotationSpeed;
    }

    if (heldKeyInputs[GLFW_KEY_S])
    {
        delta.x += deltaTime * lightRotationSpeed;
    }

    if (heldKeyInputs[GLFW_KEY_A])
    {
        delta.y -= deltaTime * lightRotationSpeed;
    }

    if (heldKeyInputs[GLFW_KEY_D])
    {
        delta.y += deltaTime * lightRotationSpeed;
    }

    if (heldKeyInputs[GLFW_KEY_Q])
    {
        delta.z += deltaTime * lightRotationSpeed;
    }

    if (heldKeyInputs[GLFW_KEY_E])
    {
        delta.z -= deltaTime * lightRotationSpeed;
    }

    light->transform.RotateAroundPoint(delta, anchorPoint);
    light->model->transform.RotateAroundPoint(delta, anchorPoint);
}

void IntensityInputs()
{
    if (heldKeyInputs[GLFW_KEY_LEFT])
    {
        lightsList[0]->lightIntensity -= dirIntensitySpeed * deltaTime;

        if (lightsList[0]->lightIntensity < 0)
        {
            lightsList[0]->lightIntensity = 0;
        }
    }

    if (heldKeyInputs[GLFW_KEY_RIGHT])
    {
        lightsList[0]->lightIntensity += dirIntensitySpeed * deltaTime;
    }

    if (heldKeyInputs[GLFW_KEY_UP])
    {
        lightsList[1]->lightIntensity += pointIntensitySpeed * deltaTime;
    }

    if (heldKeyInputs[GLFW_KEY_DOWN])
    {
        lightsList[1]->lightIntensity -= pointIntensitySpeed * deltaTime;

        if (lightsList[1]->lightIntensity < 0)
        {
            lightsList[1]->lightIntensity = 0;
        }
    }
}

void PerspectiveCamRotationInputs()
{
    // Check for clicks
    if (!heldButtonInputs[GLFW_MOUSE_BUTTON_1])
        return;

    // Get main camera but return if not in perspective mode
    Camera* perspective = mainCamera;
    if (!InstanceOf<Camera, PerspectiveCamera>(perspective))
        return;

    // Apply rotation
    mouseDir *= mouseSensitivity * deltaTime;
    glm::vec2 camPos = glm::vec2(perspective->transform.position.x, perspective->transform.position.z);
    camPos = glm::normalize(camPos);

    glm::vec3 rotate(
        mouseDir.y * -camPos.y,    // rotate.x : pitch rotation, relies on vertical movements of mouse to look up or down
        //          : also dependent on the position along the z-axis in case forward vector is pointing along the z-axis
        mouseDir.x,                // rotate.y : yaw rotation, only relies on horizontal movements of mouse to move left or right
        mouseDir.y * camPos.x      // rotate.z : roll rotation, relies on vertical movements of mouse to look up or down
    );                             //          : also dependent on the position along the x-axis in case forward vector is pointing along the x-axis

    glm::vec3 currRotation = perspective->transform.GetEulerRotation();

    if (currRotation.x + rotate.x > 60.f)
    {
        rotate.x = 60.f - currRotation.x;
    }
    else if (currRotation.x + rotate.x < -60.f)
    {
        rotate.x = -60.f - currRotation.x;
    }

    if (currRotation.z + rotate.z > 60.f)
    {
        rotate.z = 60.f - currRotation.z;
    }
    else if (currRotation.z + rotate.z < -60.f)
    {
        rotate.z = -60.f - currRotation.z;
    }

    perspective->transform.RotateAroundPoint(rotate, perspective->GetCamTarget());
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // Press only
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        isControllingLight = !isControllingLight;

        // Get reference to point light and change its color
        PointLight* light = ConvertTo<Light, PointLight>(lightsList[1]);
        glm::vec3 color = isControllingLight ? glm::vec3(1.0f, 0.3f, 0.3f) : glm::vec3(1.0f, 1.0f, 1.0f);
        light->ChangeColor(color);
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        mainCamera = camerasList[0];
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        mainCamera = camerasList[1];
    }

    // Can be held down
    if (action == GLFW_PRESS && (
        key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_D || key == GLFW_KEY_A ||
        key == GLFW_KEY_Q || key == GLFW_KEY_E ||
        key == GLFW_KEY_UP || key == GLFW_KEY_DOWN || key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT))
    {
        heldKeyInputs[key] = true;
    }
    else if (action == GLFW_RELEASE && (
        key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_D || key == GLFW_KEY_A ||
        key == GLFW_KEY_Q || key == GLFW_KEY_E ||
        key == GLFW_KEY_UP || key == GLFW_KEY_DOWN || key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT))
    {
        heldKeyInputs[key] = false;
    }
}

void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
    // Get the change in position of the cursor and calculate its direction
    oldCursorPos = currentCursorPos;
    currentCursorPos = glm::vec2(xPos, yPos);

    mouseDir = currentCursorPos - oldCursorPos;
    mouseDir = glm::normalize(mouseDir);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mod)
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
#pragma endregion

#pragma region Initialize Program
bool InitializeOpenGL(GLFWwindow** window)
{
    if (!glfwInit())
        return false;

    *window = glfwCreateWindow(width, height, "Aamir Akim", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(*window);
    gladLoadGL();
    glViewport(0, 0, width, height);

    glfwSetKeyCallback(*window, KeyCallback);
    glfwSetCursorPosCallback(*window, CursorPositionCallback);
    glfwSetMouseButtonCallback(*window, MouseButtonCallback);
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    return true;
}

String GetShaderData(String path)
{
    std::fstream src(path);
    std::stringstream buffer;
    buffer << src.rdbuf();

    return buffer.str();
}

GLuint InitializeShaderProgram(const char* vertexData, const char* fragmentData, GLuint& vertShader, GLuint& fragShader)
{
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertexData, NULL);
    glCompileShader(vertShader);

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragmentData, NULL);
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    return shaderProgram;
}
#pragma endregion

#pragma region Setup Scene
void LoadObjects(const List<String>& objPathsList)
{
    for (String path : objPathsList)
    {
        ModelReference* reference = new ModelReference(path);
        reference->LoadObject();
        modelReferencesList.push_back(reference);
    }
}

void LoadTextures(const List<Pair<String, GLuint>>& textureInfoList)
{
    for (Pair<String, GLint> info : textureInfoList)
    {
        Texture* texture = new Texture(info.first.c_str()); // pass in file path
        texture->LoadTexture(info.second);                  // pass in image format
        texturesList.push_back(texture);
    }
}

void CreateModels(const List<Pair<int, int>>& objTextureMap, const List<glm::vec3>& modelsInfoList)
{
    for (int i = 0; i < objTextureMap.size(); i++)
    {
        Pair<int, int> pair = objTextureMap[i];     // first - index to obj ref, second - index to texture

        ModelReference* objRef = modelReferencesList[pair.first];
        Texture* texture = (pair.second != -1) ? texturesList[pair.second] : NULL;

        Model3D* newModel = new Model3D(objRef, texture);
        modelsList.push_back(newModel);

        newModel->transform.position = modelsInfoList[i * 4 + 0];
        newModel->transform.Rotate(modelsInfoList[i * 4 + 1]);
        newModel->transform.scale = modelsInfoList[i * 4 + 2];
        newModel->color = modelsInfoList[i * 4 + 3];
    }
}

void CreateLights(const List<Pair<glm::vec3, unsigned int>>& lightsInfoList)
{
    for (Pair<glm::vec3, unsigned int> info : lightsInfoList)   // first - light pos, second - light type
    {
        Light* light;

        if (info.second == 0)
        {
            DirectionalLight* dirLight = new DirectionalLight();
            dirLight->transform.position = info.first;

            glm::vec3 dir = glm::vec3(0.f, 0.f, 0.f) - info.first;
            dir = glm::normalize(dir);
            dirLight->lightDir = dir;

            light = dirLight;
        }
        else if (info.second == 1)
        {
            PointLight* pointLight = new PointLight();
            pointLight->transform.position = info.first;

            pointLight->radius = 15.f;
            pointLight->model = modelsList[1];

            light = pointLight;
        }

        lightsList.push_back(light);
    }
}

void CreateCameras(const List<glm::vec3>& camerasInfoList)
{
    PerspectiveCamera* perspectiveCam = new PerspectiveCamera(fov, width, height, 0.1f, 100.f);
    perspectiveCam->ComputeProjectionMatrix();
    perspectiveCam->transform.position = camerasInfoList[0];
    perspectiveCam->transform.Rotate(camerasInfoList[1]);
    perspectiveCam->SetCamTarget(camerasInfoList[2]);
    camerasList.push_back(perspectiveCam);

    OrthoCamera* orthoCam = new OrthoCamera(-5.0f, 5.0f, -5.0f, 5.0f, -10.0f, 10.0f);
    orthoCam->ComputeProjectionMatrix();
    orthoCam->transform.position = camerasInfoList[3];
    orthoCam->transform.Rotate(camerasInfoList[4]);
    orthoCam->SetCamTarget(camerasInfoList[5]);
    camerasList.push_back(orthoCam);

    mainCamera = camerasList[0];
}
#pragma endregion

#pragma region Rendering
glm::mat4 CreateViewMatrix()
{
    glm::mat4 camPosMatrix = glm::translate(identity_matrix4, -mainCamera->transform.position);
    glm::vec3 F = glm::normalize(mainCamera->transform.localForward);
    glm::vec3 R = glm::normalize(glm::cross(worldUp, F));
    glm::vec3 U = glm::normalize(glm::cross(F, R));

    glm::mat4 camOrientation = glm::mat4(1.0f);
    camOrientation[0][0] = R.x;
    camOrientation[1][0] = R.y;
    camOrientation[2][0] = R.z;
    camOrientation[0][1] = U.x;
    camOrientation[1][1] = U.y;
    camOrientation[2][1] = U.z;
    camOrientation[0][2] = -F.x;
    camOrientation[1][2] = -F.y;
    camOrientation[2][2] = -F.z;

    return camOrientation * camPosMatrix;
}

void ComputeVerticesWithShaders(GLuint& shaderProgram, glm::mat4& transformationMatrix, glm::mat4& viewMatrix)
{
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(mainCamera->GetProjectionMatrix()));
}

void ComputeFragmentsWithShaders(GLuint& shaderProgram, Texture* texture, glm::vec3& color)
{
    GLuint isOnlyColorAddress = glGetUniformLocation(shaderProgram, "isOnlyColor");
    glUniform1i(isOnlyColorAddress, (int)(texture == NULL));

    unsigned int colorShader = glGetUniformLocation(shaderProgram, "newColor");
    glUniform3fv(colorShader, 1, glm::value_ptr(color));

    if (texture != NULL)
    {
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
        glUniform1i(tex0Address, 0);
    }

    GLuint camPosAddress = glGetUniformLocation(shaderProgram, "camPos");
    glUniform3fv(camPosAddress, 1, glm::value_ptr(mainCamera->transform.position));


    PassLightingData(shaderProgram);
}

void PassLightingData(GLuint& shaderProgram)
{
    for (Light* light : lightsList)
    {
        if (InstanceOf<Light, DirectionalLight>(light))
        {
            DirectionalLight* dirLight = ConvertTo<Light, DirectionalLight>(light);

            GLuint dirAddress = glGetUniformLocation(shaderProgram, "dirLight.dir");
            glUniform3fv(dirAddress, 1, glm::value_ptr(dirLight->lightDir));
            GLuint colorAddress = glGetUniformLocation(shaderProgram, "dirLight.color");
            glUniform3fv(colorAddress, 1, glm::value_ptr(dirLight->lightColor));
            GLuint intensityAddress = glGetUniformLocation(shaderProgram, "dirLight.intensity");
            glUniform1f(intensityAddress, dirLight->lightIntensity);

            GLuint ambientStrAddress = glGetUniformLocation(shaderProgram, "dirLight.ambientStr");
            glUniform1f(ambientStrAddress, dirLight->ambientStr);
            GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "dirLight.ambientColor");
            glUniform3fv(ambientColorAddress, 1, glm::value_ptr(dirLight->ambientColor));

            GLuint specStrAddress = glGetUniformLocation(shaderProgram, "dirLight.specStr");
            glUniform1f(specStrAddress, dirLight->specStr);
            GLuint specPhongAddress = glGetUniformLocation(shaderProgram, "dirLight.specPhong");
            glUniform1f(specPhongAddress, dirLight->specPhong);
        }
        else if (InstanceOf<Light, PointLight>(light))
        {
            PointLight* pointLight = ConvertTo<Light, PointLight>(light);

            GLuint radiusAddress = glGetUniformLocation(shaderProgram, "pointLight.radius");
            glUniform1f(radiusAddress, pointLight->radius);
            GLuint positionAddress = glGetUniformLocation(shaderProgram, "pointLight.pos");
            glUniform3fv(positionAddress, 1, glm::value_ptr(pointLight->transform.position));
            GLuint colorAddress = glGetUniformLocation(shaderProgram, "pointLight.color");
            glUniform3fv(colorAddress, 1, glm::value_ptr(pointLight->lightColor));
            GLuint intensityAddress = glGetUniformLocation(shaderProgram, "pointLight.intensity");
            glUniform1f(intensityAddress, pointLight->lightIntensity);

            GLuint ambientStrAddress = glGetUniformLocation(shaderProgram, "pointLight.ambientStr");
            glUniform1f(ambientStrAddress, pointLight->ambientStr);
            GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "pointLight.ambientColor");
            glUniform3fv(ambientColorAddress, 1, glm::value_ptr(pointLight->ambientColor));

            GLuint specStrAddress = glGetUniformLocation(shaderProgram, "pointLight.specStr");
            glUniform1f(specStrAddress, pointLight->specStr);
            GLuint specPhongAddress = glGetUniformLocation(shaderProgram, "pointLight.specPhong");
            glUniform1f(specPhongAddress, pointLight->specPhong);
        }
    }
}
#pragma endregion


// Amumu: https://www.models-resource.com/pc_computer/leagueoflegends/model/23225/
// Sphere: exported from the primitive sphere of Unity