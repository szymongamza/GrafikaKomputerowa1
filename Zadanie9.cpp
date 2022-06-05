#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>


const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 normal;\n"
"out vec3 fragmentPosition;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    fragmentPosition = vec3(model* vec4(position,1.0));\n"
"    Normal = mat3(transpose(inverse(model))) * normal;\n"
"    gl_Position = projection * view * model * vec4(position, 1.0);\n"
"}\0";

const GLchar* vertexShaderLightSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(position, 1.0);\n"
"}\0";

const GLchar* fragmentShaderLightSource =
"#version 330 core\n"
"out vec4 fragmentColor;\n"
"void main()\n"
"{\n"
"    fragmentColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource =
"#version 330 core\n"
"in vec3 Normal;\n"
"in vec3 fragmentPosition;\n"
"out vec4 fragmentColor;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform float ambientStrength;\n"
"uniform float specularStrength;\n"
"uniform float diffuseStrength;\n"
"vec3 objectColor = vec3(0.0, 1.0, 0.0);\n"
"vec3 lightColor = vec3(1.0, 1.0, 1.0);\n"
"void main()\n"
"{\n"
"    vec3 ambientColor = ambientStrength * lightColor;\n"
"    vec3 norm = normalize(Normal);\n"
"    vec3 lightDir = normalize(lightPos - fragmentPosition);\n"
"    float diff = max(dot(norm, lightDir), 0.0);\n"
"    vec3 diffuse = diff * lightColor * diffuseStrength;\n"
"    vec3 viewDir = normalize(viewPos - fragmentPosition);\n"
"    vec3 reflectDir = reflect(-lightDir, norm);\n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);\n"
"    vec3 specular = specularStrength * spec * lightColor;\n"
"    vec3 result = (ambientColor + diffuse + specular) * objectColor;\n"
"    fragmentColor = vec4(result, 1.0);\n"
"}\0";

void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void TitleWindow(GLFWwindow* window);

float pitch = 0.0f;
float yaw = -90.0f;
const unsigned int window_width = 1000;
const unsigned int window_height = 1000;
float previousX = static_cast<float>(window_width) / 2.0f;
float previousY = static_cast<float>(window_height) / 2.0f;
bool first_window_enter = true;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 2.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float previousTime = 0.0f;
float degrees = 0;
float deltaMotion = 0.0f;
glm::vec3 lightPosition(0.0f, 2.0f, 0.0f);

bool ambient = true;
bool diffuse = true;
bool spec = true;


int main()
{
    // inicjalizacja GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Tworzenie okna

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "kamera", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


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
    /// <summary>
    /// </summary>
    /// <returns></returns>
    GLuint lightVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(lightVertexShader, 1, &vertexShaderLightSource, NULL);
    glCompileShader(lightVertexShader);

    glGetShaderiv(lightVertexShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(lightVertexShader, 512, NULL, error_message);
        std::cout << "Error (Vertex shader): " << error_message << std::endl;
    }

    GLuint lightFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(lightFragmentShader, 1, &fragmentShaderLightSource, NULL);
    glCompileShader(lightFragmentShader);

    glGetShaderiv(lightFragmentShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(lightFragmentShader, 512, NULL, error_message);
        std::cout << "Error (Fragment shader): " << error_message << std::endl;
    }

    GLuint lightShaderProgram = glCreateProgram();
    glAttachShader(lightShaderProgram, lightVertexShader);
    glAttachShader(lightShaderProgram, lightFragmentShader);
    glLinkProgram(lightShaderProgram);

    glGetProgramiv(lightShaderProgram, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(lightShaderProgram, 512, NULL, error_message);
        std::cout << "Error (Shader program): " << error_message << std::endl;
    }


    glDetachShader(lightShaderProgram, lightVertexShader);
    glDetachShader(lightShaderProgram, lightFragmentShader);



    glDeleteShader(lightVertexShader);
    glDeleteShader(lightFragmentShader);

    /*
    // vertex data
    GLfloat vertices[] = {
        // coordinates          // colors
        -0.8f,  0.6f, 0.0f,     0.0f, 1.0f, 0.0f, // triangle 1,2 vertex 1, 1
         0.8f, -0.6f, 0.0f,     0.0f, 1.0f, 0.0f, // triangle 1,2 vertex 2, 3
        -0.8f, -0.6f, 0.0f,     0.0f, 1.0f, 0.0f, // triangle 1   vertex 3
         0.8f,  0.6f, 0.0f,     0.0f, 1.0f, 0.0f  // triangle 2   vertex 2
    };

    GLuint indices[] = {
        0, 1, 2,    // triangle 1
        0, 3, 1     // triangle 2
    };
    */
    GLfloat vertices[] = {
        // vertices coords      // normals 
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f
    };



    GLuint indices[] = {
        0,1,2,
        3,4,5,
        6,7,8,
        9,10,11,
        12,13,14,
        15,16,17,
        18,19,20,
        21,22,23,
        24,25,26,
        27,28,29,
        30,31,32,
        33,34,35
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    GLuint lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, (GLuint)window_width, (GLuint)window_height);

    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetTime(0.0);
    // pêtla zdarzeñ
    while (!glfwWindowShouldClose(window))
    {
        glUseProgram(shaderProgram);
        // renderowanie 1 cube
        glClearColor(0.066f, 0.09f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLint lightPositionLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glm::vec3 lightPosition((cos(glfwGetTime()) * 3), 2.0f, (sin(glfwGetTime()) * 3));
        glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);


        GLint cameraPositionLoc = glGetUniformLocation(shaderProgram, "viewPos");
        glUniform3f(cameraPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

        GLint diffuseLoc = glGetUniformLocation(shaderProgram, "diffuseStrength");
        if (diffuse) {
            glUniform1f(diffuseLoc, 1.0f);
        }
        else {
            glUniform1f(diffuseLoc, 0.0f);
        }
        GLint ambientLoc = glGetUniformLocation(shaderProgram, "ambientStrength");
        if (ambient) {
            glUniform1f(ambientLoc, 0.15f);
        }
        else {
            glUniform1f(ambientLoc, 0.0f);
        }
        GLint specularLoc = glGetUniformLocation(shaderProgram, "specularStrength");
        if (spec) {
            glUniform1f(specularLoc, 0.4f);
        }
        else {
            glUniform1f(specularLoc, 0.0f);
        }




        glm::mat4 model = glm::mat4(1.0f);

        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


        glm::vec3 cameraFront_new;
        cameraFront_new.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront_new.y = sin(glm::radians(pitch));
        cameraFront_new.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(cameraFront_new);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window_width) / static_cast<float>(window_height), 0.1f, 100.0f);

        GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));




        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // renderowanie 2 cube
        
        glUseProgram(lightShaderProgram);

        model = glm::mat4(1.0f);
        modelLoc = glGetUniformLocation(lightShaderProgram, "model");
        model = glm::translate(model, glm::vec3(lightPosition.x, lightPosition.y, lightPosition.z));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        viewLoc = glGetUniformLocation(lightShaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        projectionLoc = glGetUniformLocation(lightShaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        

        //model = glm::translate(model, glm::vec3((cos(glfwGetTime()) *2), 1.5f, (sin(glfwGetTime()) *2)));
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

        glfwSwapBuffers(window);
        glfwPollEvents();
        float currentTime = glfwGetTime();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;
        TitleWindow(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (first_window_enter) {
        previousX = xpos; previousY = ypos;
        first_window_enter = false;
    }

    float xdifference = xpos - previousX;
    float ydifference = previousY - ypos;
    previousX = xpos;
    previousY = ypos;
    const float sensitivity = 0.075f;
    xdifference *= sensitivity;
    ydifference *= sensitivity;
    pitch += ydifference;
    yaw += xdifference;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPosition += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPosition -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        ambient = !ambient;
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        diffuse = !diffuse;
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        spec = !spec;
    }
}

void TitleWindow(GLFWwindow* window) {
    std::ostringstream ss;
    ss << "ambient: " << ambient << " diffuse: " << diffuse << " spec: " << spec;
    std::string s(ss.str());

    const char* c = s.c_str();
    glfwSetWindowTitle(window, c);
}
