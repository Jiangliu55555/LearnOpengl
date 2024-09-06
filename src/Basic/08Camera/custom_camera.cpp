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

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char* vertexPath = "src/Basic/08Camera/simple.vs";
const char* fragmentPath = "src/Basic/08Camera/simple.fs";


// shader uniforms
const char* intensityStr = "_Intensity";
// sampler2D uniform 0
const char* texture1Str = "_Texture1";
// sampler2D uniform 1
const char* texture2Str = "_Texture2";

const char* M_Mat4Str = "M_Matrix";
const char* V_Mat4Str = "V_Matrix";
const char* P_Mat4Str = "P_Matrix";

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

    // Initialize shader
    Shader shaderProgram(FileSystem::getPath(vertexPath).c_str(), FileSystem::getPath(fragmentPath).c_str());

    // set up vertex data (and buffer)
    // Cube
    float vertices[] = {
            // front face
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left
            // right face
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
            0.5f, -0.5f, -1.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
            0.5f,  0.5f, -1.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
            // left face
            -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
            -0.5f, -0.5f, -1.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
            -0.5f,  0.5f, -1.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
            // top face
            0.5f,  0.5f, -1.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
            -0.5f, 0.5f, -1.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
            -0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
            0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
            // bottom face
            0.5f, -0.5f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
            -0.5f, -0.5f, -1.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
            
            // back face
            0.5f,  0.5f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
            0.5f, -0.5f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
            -0.5f, -0.5f, -1.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
            -0.5f,  0.5f, -1.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    };
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3,  // second triangle
            4, 5, 7,
            5, 6, 7,
            8, 9, 11,
            9, 10, 11,
            12, 13, 15,
            13, 14, 15,
            16, 17, 19,
            17, 18, 19,
            20, 21, 23,
            21, 22, 23,
    };

    unsigned int VBO, VAO, EBO;
    // generate VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Shader Attributes
    // first 0 = shader layout location
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // enable vertex attribute array
    glEnableVertexAttribArray(0);


    // vertex color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//    glBindBuffer(GL_ARRAY_BUFFER, 0); // no need to unbind VBO, as bindVertexArray binds it to itself

    // load and create texture
    // texture 1
    // generate texture
    unsigned int texture1;
    glGenTextures(1, &texture1);
    // bind texture
    glBindTexture(GL_TEXTURE_2D, texture1);

    // texture wrapping params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // texture filtering params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* data = stbi_load(FileSystem::getPath("Res/Textures/MuBan.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // set texture parameters
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    // free image data
    stbi_image_free(data);

    // texture 2
    // generate texture
    unsigned int texture2;
    glGenTextures(1, &texture2);
    // bind texture
    glBindTexture(GL_TEXTURE_2D, texture2);

    // texture wrapping params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // texture filtering params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load(FileSystem::getPath("Res/Textures/shitou.png").c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        // set texture parameters
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        // free image data
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    shaderProgram.use();
    shaderProgram.setFloat(intensityStr, 1.0f);
//    glUniform1i(glGetUniformLocation(shaderProgram.ID, texture1Str), 0);
//    glUniform1i(glGetUniformLocation(shaderProgram.ID, texture2Str), 1);

    shaderProgram.setInt(texture1Str, 0);
    shaderProgram.setInt(texture2Str, 1);

    glEnable(GL_DEPTH_TEST); // enable depth testing
//    unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "_Transform");
//    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    // render loop

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
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

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // activate shader program
        // glUseProgram(shaderProgram);
       

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // float timeValue = glfwGetTime();
        // float greenValue = sin(timeValue) * 0.5f + 0.5f;
        // int baseColorId = glGetUniformLocation(shaderProgram, "_BaseColor");
        // glUniform4f(baseColorId, 0.0f, greenValue, 0.0f, 1.0f);

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
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // GL_UNSIGNED_INT = 4 bytes per index
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 m_matrix = glm::mat4(1.0f);
            m_matrix = glm::translate(m_matrix, cubePositions[i]);
            m_matrix = glm::rotate(m_matrix, glm::radians(45.0f * (i + 1)), glm::vec3(1.0f, 0.3f, 0.5f));
            shaderProgram.setMat4(M_Mat4Str, m_matrix);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        // deactivate shader program
        glUseProgram(0);
        glfwSwapBuffers(window);
    }
    // release resources
    glfwTerminate();

    // delete shaders (no longer needed)

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);


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
    camera.ProcessMouseScroll(yoffset * 0.1f);
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

