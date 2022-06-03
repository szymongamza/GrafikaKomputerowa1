#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <iostream>
using namespace std;


const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 color;\n"
"layout(location = 2) in vec2 texture;\n"
"out vec3 vertexColor;\n"
"out vec2 vertexTexture;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"    vertexColor = color;\n"
"    vertexTexture = texture;\n"
"}\0";

const GLchar* fragmentShaderSource =
"#version 330 core\n"
"in vec3 vertexColor;\n"
"in vec2 vertexTexture;\n"
"out vec4 fragmentColor;\n"
"uniform sampler2D uTexture;\n"
"uniform float textureMix = 0.05f; \n"
"void main()\n"
"{\n"
"    fragmentColor = mix(texture(uTexture, vertexTexture), vec4(0.0, 0.0, 1.0, 1.0), textureMix);\n"
"}\0";

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

GLint glDXLocation;

float fade = 0.05f;
int option = 3;

int main()
{
    // inicjalizacja GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Tworzenie okna
    const unsigned int window_width = 1000;
    const unsigned int window_height = 1000;
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Trojkat i prostokat z tekstura", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // inicjalizacja GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // shadery
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint status;
    GLchar error_message[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, error_message);
        std::cout << "Error (Vertex shader): " << error_message << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, error_message);
        std::cout << "Error (Fragment shader): " << error_message << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDXLocation = glGetUniformLocation(shaderProgram, "textureMix");

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, error_message);
        std::cout << "Error (Shader program): " << error_message << std::endl;
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    GLubyte* tex1 = stbi_load("texture1.jpg", &width, &height, &nrChannels, 0);

    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width2, height2, nrChannels2;
    stbi_set_flip_vertically_on_load(true);
    GLubyte* tex2 = stbi_load("texture2.jpg", &width2, &height2, &nrChannels2, 0);

    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, tex2);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLfloat verticesRec[] = {
       
        -0.6f,  0.5f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 
        -0.2f,  0.2f, 0.0f,     0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 
        -0.6f,  0.2f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 
        -0.2f,  0.5f, 0.0f,     0.0f, 0.0f, 0.0f,   1.0f, 1.0f  
    };

    GLuint indicesRec[] = {
        0, 1, 2,   
        0, 3, 1    
    };

    GLfloat verticesTria[] = {
       
        0.5f,   0.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 
        0.2f,  -0.3f, 0.0f,     0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 
        0.8f,  -0.3f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 
    };

    GLuint indicesTria[] = {
        0, 1, 2
    };

    //VAO EBO VBO for Rectangle
    GLuint VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

    GLuint EBO1;
    GLuint VBO1;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRec), verticesRec, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesRec), indicesRec, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    //VAO EBO VBO for Triangle
    GLuint VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    GLuint EBO2;
    GLuint VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTria), verticesTria, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesTria), indicesTria, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    glViewport(0, 0, (GLuint)window_width, (GLuint)window_height);

    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // petla
    while (!glfwWindowShouldClose(window))
    {
        // renderowanie
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (option == 0) {
            glBindTexture(GL_TEXTURE_2D, texture1);
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO1);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        else if (option == 1) {
            glBindTexture(GL_TEXTURE_2D, texture2);
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO2);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        else if (option == 2) {
            glBindTexture(GL_TEXTURE_2D, texture1);
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO1);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            glBindTexture(GL_TEXTURE_2D, texture2);
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO2);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        else if (option == 3) {

        }

        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &EBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO2);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        option = 0;
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        option = 1;
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        option = 2;
    }
    if (action == GLFW_RELEASE) {
        option = 3;
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset > 0 && fade < 1.0f) {
        fade += 0.05f;
    }
    else if (yoffset < 0 && fade  > 0.0f) {
        fade -= 0.05f;
    }

    //DEBUG std::cout << fade << std::endl;
    glUniform1f(glDXLocation, fade);
}