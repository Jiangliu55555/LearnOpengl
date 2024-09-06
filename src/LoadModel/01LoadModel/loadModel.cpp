//
// Created by admin on 2024/8/16.
//
#include <iostream>
#include <cmath>
#include <glad/glad.h> // include glad to get all the OpenGL function pointers and load the OpenGL functions
#include <GLFW/glfw3.h> // GL Context and window handling library
#include <learnopengl/shader.h> // include our shader loading functions
#include <learnopengl/filesystem.h> // file system loading functions
#include <learnopengl/camera.h> // camera class
#include <stb_image.h> // image loading library
// glm library for matrix operations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <learnopengl/model.h>


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char* vertexPath = "src/LoadModel/01LoadModel/simple.vs";
const char* fragmentPath = "src/LoadModel/01LoadModel/simple.fs";
const char* modelPath = "Res/Model/nanosuit/nanosuit.obj";


// shader uniforms
const char* intensityStr = "_Intensity";
// sampler2D uniform 0
const char* texture1Str = "_Texture1";
// sampler2D uniform 1
const char* texture2Str = "_Texture2";

const char* M_Mat4Str = "model";
const char* V_Mat4Str = "view";
const char* P_Mat4Str = "projection";

// camera attributes
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

double lastX = 400, lastY = 300;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessInput(GLFWwindow* window);

int main()
{
    camera = Camera(cameraPos, cameraUp);

    // initialize GLFW
    glfwInit();
    // set version 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // with 800x600 window size

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // set window as current context
    glfwMakeContextCurrent(window);
    // set framebuffer size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // mouse callback
    glfwSetCursorPosCallback(window, mouse_callback);
    // scroll callback
    glfwSetScrollCallback(window, scroll_callback);
    // disable cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    stbi_set_flip_vertically_on_load(true); 

    // Initialize shader
    Shader shaderProgram(FileSystem::getPath(vertexPath).c_str(), FileSystem::getPath(fragmentPath).c_str());

    Model model(FileSystem::getPath(modelPath));

    glEnable(GL_DEPTH_TEST); // enable depth testing

    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        // timing
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // check input events
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();
        // MVP matrix
        // currentglm version must initialize matrix with identity matrix, otherwise screen will be black
       
        // float radius = 10.0f;
        // float camX = sin(glfwGetTime()) * radius;
        // float camZ = cos(glfwGetTime()) * radius;
        // cameraPos, cameraFront, cameraUp
        glm::mat4 v_matrix = camera.GetViewMatrix();
        shaderProgram.setMat4(V_Mat4Str, v_matrix);

        glm::mat4 p_matrix = glm::mat4(1.0f);
        p_matrix = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        shaderProgram.setMat4(P_Mat4Str, p_matrix);

        // draw triangle
//        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // GL_UNSIGNED_INT = 4 bytes per index
        glm::mat4 m_matrix = glm::mat4(1.0f);
        shaderProgram.setMat4(M_Mat4Str, m_matrix);

        model.Draw(shaderProgram);
        
        // deactivate shader program
        glUseProgram(0);
        glfwSwapBuffers(window);
    }
    // release resources
    glfwTerminate();

    // delete shaders (no longer needed)

//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//    glDeleteVertexArrays(1, &VAO);


    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {   
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void ProcessInput(GLFWwindow* window)
{
    // ESC to quit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // camera controls
    // wasd to move
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
        
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
        
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    
}

