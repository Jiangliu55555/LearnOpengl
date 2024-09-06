//
// Created by admin on 2024/8/16.
//
#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// const char* vertexShader = "#version 330 core\n"
//                            "layout (location = 0) in vec3 aPos;\n"
//                            "layout (location = 1) in vec3 vertexColor;\n"
//                            "out vec3 color;\n"
//                            "void main()\n"
//                            "{\n"
//                            "   gl_Position = vec4(aPos.xyz, 1.0);\n"
//                            "   color = vertexColor;\n"
//                            "}\0";
// const char* fragmentShader = "#version 330 core\n"
//                              "out vec4 FragColor;\n"
//                             //  "uniform vec4 _BaseColor;\n"
//                             "in vec3 color;\n"
//                              "void main()\n"
//                              "{\n"
//                              "   FragColor = vec4(color, 1.0f);\n"
//                              "}\n\0";

const char* vertexPath = "C:/CWorkSpace/LearnOpenGL/src/Basic/04Shader/simple.vs";
const char* fragmentPath = "C:/CWorkSpace/LearnOpenGL/src/Basic/04Shader/simple.fs";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

int main()
{
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

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Initialize shader
    Shader shaderProgram(vertexPath, fragmentPath);

    // set up vertex data (and buffer)
    float vertices[] = {
            // positions        // colors
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
    };

    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,
            1, 2, 3
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // enable vertex attribute array
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); // no need to unbind VBO, as bindVertexArray binds it to itself

    const char* intensityStr = "_Intensity";

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        // check input events
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // activate shader program
        // glUseProgram(shaderProgram);
        shaderProgram.use();
        shaderProgram.setFloat(intensityStr, 0.5f);

        // float timeValue = glfwGetTime();
        // float greenValue = sin(timeValue) * 0.5f + 0.5f;
        // int baseColorId = glGetUniformLocation(shaderProgram, "_BaseColor");
        // glUniform4f(baseColorId, 0.0f, greenValue, 0.0f, 1.0f);

        // draw triangle
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // GL_UNSIGNED_INT = 4 bytes per index
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

void ProcessInput(GLFWwindow* window)
{
    // ESC to quit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}

