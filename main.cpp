#include "pch.h"
#include "Shader.h"
#include "ModelReference.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "OrthoCamera.h"
#include "PerspectiveCamera.h"
#include "Skybox.h"
#include "Player.h"


#pragma region Global Variables
// Shaders, Models, Textures, Lights, Cameras, the Skybox, and the Player
List<Shader*> shadersList;
List<ModelReference*> modelReferencesList; 
List<Model3D*> modelsList;
List<Texture*> texturesList;
List<Light*> lightsList;
List<Camera*> camerasList;
Camera* mainCamera;
Skybox* skybox;
Player* player; 

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
bool InitializeOpenGL(GLFWwindow** window);
void LoadShaders(const List<Pair<String, String>>& shaderPathsList); 
void LoadObjects(const List<String>& objPathsList);
void LoadTextures(const List<Pair<String, GLuint>>& textureInfoList);
void CreateModels(const List<Pair<int, int>>& objTextureMap, const List<glm::vec3>& modelsInfoList);
void CreateLights(const List<Pair<glm::vec3, unsigned int>>& lightsInfoList);
void CreateCameras(const List<glm::vec3>& camerasInfoList);

glm::mat4 CreateViewMatrix();
void UpdateGround();
void UpdateMainCamera();
void ResetCameras();
void ComputeVerticesWithShaders(GLuint& shaderProgram, glm::mat4& transformationMatrix, glm::mat4& viewMatrix);
void ComputeFragmentsWithShaders(GLuint& shaderProgram, Texture* texture, glm::vec3& color);
void PassLightingData(GLuint& shaderProgram);
#pragma endregion


int main(void)
{
    // Create Player
    player = new Player(); 

    // Initialize program
    srand(time(0));
    GLFWwindow* window;
    if (!InitializeOpenGL(&window))
        return -1;

    // Load shaders 
    List<Pair<String, String>> shaderPathsList = {
        {"Shaders/default.vert", "Shaders/default.frag"},
        {"Shaders/skybox.vert", "Shaders/skybox.frag"}
    };
    LoadShaders(shaderPathsList);
    

    // Intialize Skybox
    List<String> facesSkyboxArray{
        "Skybox/CosmicCoolCloudRight.png",
        "Skybox/CosmicCoolCloudLeft.png",
        "Skybox/CosmicCoolCloudTop.png",
        "Skybox/CosmicCoolCloudBottom.png",
        "Skybox/CosmicCoolCloudFront.png",
        "Skybox/CosmicCoolCloudBack.png",
    };

    skybox = new Skybox(&shadersList[1]->GetShaderProgram());
    skybox->IntializeSkybox(facesSkyboxArray);


    // Load 3d object references
    List<String> objPathsList = {
        "3D/plane.obj",
        "3D/tank-red.obj",
        "3D/carrier-tank.obj",
        "3D/penguin.obj",
        "3D/panda.obj",
        "3D/snowman.obj",
        "3D/veigar.obj",
        "3D/amumu.obj",
    };
    LoadObjects(objPathsList);


    // Load textures
    List<Pair<String, GLuint>> textureInfoList = {
        {"3D/ground.png", GL_RGBA},
        {"3D/tank-red.png", GL_RGBA},
        {"3D/carrier-tank.png", GL_RGBA},
        {"3D/penguin.png", GL_RGBA},
        {"3D/panda.png", GL_RGBA},
        {"3D/snowman.png", GL_RGBA},
        {"3D/veigar.png", GL_RGBA},
        {"3D/amumu.png", GL_RGBA},
    };
    LoadTextures(textureInfoList);

    // Create new models
    List<Pair<int, int>> objTextureMap = {
        {0, 0},       // index to obj reference (plane.obj), index to texture (ground.png)
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 4},
        {5, 5},
        {6, 6},
        {7, 7},
    };
    List<glm::vec3> modelsInfoList = {
        // plane
        glm::vec3(0.f, 0.f, 0.f),         // position
        glm::vec3(0.f, 180.f, 0.f),       // rotation
        glm::vec3(15.f, 1.f, 15.f),       // scale
        glm::vec3(1.f, 1.f, 1.f),         // color
        // player tank
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(1.f, 1.f, 1.f),
        glm::vec3(1.f, 1.f, 1.f),
        // carrier tank
        glm::vec3(25.f, 0.f, 25.f),
        glm::vec3(0.f, 0.f, 0.f), 
        glm::vec3(0.001f, 0.001f, 0.001f),
        glm::vec3(1.f, 1.f, 1.f),
        // penguin
        glm::vec3(-25.f, 0.f, -25.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.02f, 0.02f, 0.02f),
        glm::vec3(1.f, 1.f, 1.f),
        // panda
        glm::vec3(25.f, 0.f, -25.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.f, 1.f, 1.f),
        // snowman
        glm::vec3(-25.f, 0.f, 25.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(1.f, 1.f, 1.f),
        // veigar
        glm::vec3(50.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.01f, 0.01f, 0.01f),
        glm::vec3(1.f, 1.f, 1.f),
        // amumu
        glm::vec3(-50.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(1.f, 1.f, 1.f),
    };
    CreateModels(objTextureMap, modelsInfoList);


    // Create Lights
    List<Pair<glm::vec3, unsigned int>> lightsInfoList = {
        {glm::vec3(0.f, 0.f, 0.f), 0},              // position, light type
        {glm::vec3(0.f, 0.5f, 1.f), 1},
    };
    CreateLights(lightsInfoList);


    // Create Cameras
    List<glm::vec3> camerasInfoList = {
        // third-person
        glm::vec3(0.f, 6.0f, -6.f),           // position
        glm::vec3(45.f, 0.f, 0.f),            // rotation
        modelsList[1]->transform.position,    // camera target 
        // first-person
        glm::vec3(0.f, 1.f, 1.f),
        modelsList[1]->transform.GetEulerRotation(),
        modelsList[1]->transform.localForward,
        // ortho
        glm::vec3(0.f, 8.f, 0.f),
        glm::vec3(89.f, 0.f, 0.f),
        modelsList[1]->transform.position,
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
        UpdateGround();
        UpdateMainCamera();
        player->CheckInputs(deltaTime, mainCamera);


        // Iterate through modelsList
        for (Model3D* model : modelsList)
        {
            glm::mat4 transformationMatrix = model->GetTransformationMatrix();
            glm::mat4 viewMatrix = CreateViewMatrix(); 

            ComputeVerticesWithShaders(shadersList[0]->GetShaderProgram(), transformationMatrix, viewMatrix);
            ComputeFragmentsWithShaders(shadersList[0]->GetShaderProgram(), model->texture, model->color);

            skybox->Render(viewMatrix, mainCamera->GetProjectionMatrix(), (mainCamera == camerasList[1])); 

            glUseProgram(shadersList[0]->GetShaderProgram());
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

    glfwSetWindowUserPointer(*window, &player); 

    glfwSetKeyCallback(*window, [](GLFWwindow* window, int key, int scancode, int action, int mod) {
        player->KeyCallback(window, key, scancode, action, mod); 
    });
    glfwSetCursorPosCallback(*window, [](GLFWwindow* window, double xPos, double yPos) {
        player->CursorPositionCallback(window, xPos, yPos);
    });
    glfwSetMouseButtonCallback(*window, [](GLFWwindow* window, int button, int action, int mod) {
        player->MouseButtonCallback(window, button, action, mod);
    });
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    return true;
}

void LoadShaders(const List<Pair<String, String>>& shaderPathsList)
{
    for (Pair<String, String> pair : shaderPathsList)
    {
        String vert = pair.first;
        String frag = pair.second;

        Shader* shader = new Shader();
        shader->InitializeProgram(vert, frag);
        shadersList.push_back(shader);
    }
}

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

    player->SetPlayerModel(modelsList[1]);
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
            dirLight->lightColor = glm::vec3(0.5f, 0.5f, 0.8f);
            dirLight->ambientColor = glm::vec3(0.2f, 0.4f, 0.8f);
            dirLight->lightIntensity = 0.05f;

            glm::vec3 dir = glm::vec3(0.25f, -1.f, -0.5f);
            dir = glm::normalize(dir);
            dirLight->lightDir = dir;

            light = dirLight;
        }
        else if (info.second == 1)
        {
            PointLight* pointLight = new PointLight();
            pointLight->transform.position = info.first;

            pointLight->radius = 20.f;
            pointLight->model = modelsList[1];

            light = pointLight;
        }

        lightsList.push_back(light);
    }

    player->SetLight(lightsList[1]);
}

void CreateCameras(const List<glm::vec3>& camerasInfoList)
{
    PerspectiveCamera* thirdPerson = new PerspectiveCamera(70.f, width, height, 0.1f, 50.f);
    thirdPerson->ComputeProjectionMatrix(); 
    thirdPerson->transform.position = camerasInfoList[0]; 
    thirdPerson->transform.Rotate(camerasInfoList[1]); 
    thirdPerson->SetCamTarget(camerasInfoList[2]); 
    camerasList.push_back(thirdPerson); 

    PerspectiveCamera* firstPerson = new PerspectiveCamera(fov, width, height, 0.6f, 100.f);
    firstPerson->ComputeProjectionMatrix(); 
    firstPerson->transform.position = camerasInfoList[3]; 
    firstPerson->transform.Rotate(camerasInfoList[4]); 
    firstPerson->SetCamTarget(camerasInfoList[5]); 
    camerasList.push_back(firstPerson);

    OrthoCamera* orthoCam = new OrthoCamera(-15.0f, 15.0f, -15.0f, 15.0f, -10.0f, 10.0f);
    orthoCam->ComputeProjectionMatrix();
    orthoCam->transform.position = camerasInfoList[6];
    orthoCam->transform.Rotate(camerasInfoList[7]);
    orthoCam->SetCamTarget(camerasInfoList[8]);
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

void UpdateGround()
{
    Model3D* ground = modelsList[0];

    if (player->GetCurrentCamera() == Cameras::Birds_Eye)
    {
        glm::vec3 pos = camerasList[2]->transform.position;
        pos.y = 0;
        ground->transform.position = pos;
    }
    else
    {
        ground->transform.position = player->transform.position;
    }
}

void UpdateMainCamera()
{
    Cameras currentCamera = player->GetCurrentCamera();

    if (currentCamera == Cameras::Third_Person && mainCamera != camerasList[0])
    {
        mainCamera = camerasList[0];
        ResetCameras(); 
    }
    else if (currentCamera == Cameras::First_Person && mainCamera != camerasList[1])
    {
        mainCamera = camerasList[1];
        ResetCameras(); 
    }
    else if (currentCamera == Cameras::Birds_Eye && mainCamera != camerasList[2])
    {
        mainCamera = camerasList[2];
        ResetCameras();
    }
}

void ResetCameras()
{
    PerspectiveCamera* firstPersonCam = ConvertTo<Camera, PerspectiveCamera>(camerasList[1]);
    glm::vec3 displacement = player->transform.localForward;
    displacement.y = 1.f;
    firstPersonCam->transform.position = player->transform.position + displacement;

    glm::vec3 rot = glm::vec3(0.f, player->transform.GetEulerRotation().y, 0.f);
    firstPersonCam->transform.ResetRotation(); 
    firstPersonCam->transform.RotateAroundPoint(rot, firstPersonCam->transform.position);

    firstPersonCam->fov = fov;
    firstPersonCam->ComputeProjectionMatrix(); 
    

    Camera* birdsEyeCam = camerasList[2];
    displacement = birdsEyeCam->transform.position;
    displacement.x = 0.f;
    displacement.z = 0.f;
    birdsEyeCam->transform.position = player->transform.position + displacement;

    rot = glm::vec3(0.f, player->transform.GetEulerRotation().y, 0.f);
    rot.x = birdsEyeCam->transform.GetEulerRotation().x; 
    birdsEyeCam->transform.ResetRotation();
    birdsEyeCam->transform.Rotate(rot); 
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
    GLuint isUntlitAddress = glGetUniformLocation(shaderProgram, "isUntlit");
    glUniform1i(isUntlitAddress, (int)(texture == NULL));

    unsigned int colorShader = glGetUniformLocation(shaderProgram, "newColor");
    glUniform3fv(colorShader, 1, glm::value_ptr(color));

    if (texture != NULL)
    {
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
        glUniform1i(tex0Address, 0);
    }

    GLuint isNightVisionAddress = glGetUniformLocation(shaderProgram, "isNightVision");
    glUniform1i(isNightVisionAddress, (int)(mainCamera == camerasList[1]));

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


// carrier-tank: https://www.models-resource.com/pc_computer/teamfortress2/model/51616/
// tanks: https://www.models-resource.com/gamecube/gotchaforce/model/51842/    
// Amumu: https://www.models-resource.com/pc_computer/leagueoflegends/model/23225/   
// Veigar: https://www.models-resource.com/pc_computer/leagueoflegends/model/23244/ 
// penguin: https://www.models-resource.com/pc_computer/bloonstd6/model/61916/  
// snowman: https://www.models-resource.com/pc_computer/bloonstd6/model/61919/ 
// panda: https://www.models-resource.com/mobile/burritobashwebarebears/model/28212/  
// skybox: https://assetstore.unity.com/packages/2d/textures-materials/sky/skybox-series-free-103633