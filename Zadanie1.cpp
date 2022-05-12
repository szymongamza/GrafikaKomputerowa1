#include <glad/glad.h>  // musi byæ do³¹czony jako pierwszy
#include <GLFW/glfw3.h>

#include <iostream>





const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"void main() {\n"
"    gl_Position = vec4(position, 1.0); // vec4(position.x, position.y, position.z, 1.0);\n"
"    vertexColor = color;\n"
"}\0";

const GLchar* fragmentShaderSource =

"#version 330 core\n"
"in vec3 vertexColor;\n"
"out vec4 fragmentColor;\n"
"void main() {\n"
"    fragmentColor = vec4(vertexColor, 1.0);\n"
"}\0";

const GLchar* vertexShaderSource1 =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"void main() {\n"
"    gl_Position = vec4(position, 1.0); // vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource1 =

"#version 330 core\n"
"out vec4 fragmentColor;\n"
"void main() {\n"
"    fragmentColor = vec4(0.0,0.0,1.0, 1.0);\n"
"}\0";




int main()
{
    // inicjalizacja GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Tworzenie okna
    const unsigned int window_width = 1000;
    const unsigned int window_height = 800;
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "grafika komputerowa", NULL, NULL);
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

    //GLSL - Open GL Shading Language - Shadery
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint status;
    GLchar error_message[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(vertexShader, 512, NULL, error_message);
        std::cout << "Error (Vertex shader): " << error_message << std::endl;
    }

    GLuint vertexShader1 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader1, 1, &vertexShaderSource1, NULL);
    glCompileShader(vertexShader1);

    glGetShaderiv(vertexShader1, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(vertexShader1, 512, NULL, error_message);
        std::cout << "Error (Vertex shader): " << error_message << std::endl;
    }



    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, error_message);
        std::cout << "Error (Vertex shader): " << error_message << std::endl;
    }

    GLuint fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShader1);

    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(fragmentShader1, 512, NULL, error_message);
        std::cout << "Error (Vertex shader): " << error_message << std::endl;
    }



    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLuint shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader1);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);


    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, error_message);
        std::cout << "Error (Vertex shader): " << error_message << std::endl;
    }

    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(shaderProgram1, 512, NULL, error_message);
        std::cout << "Error (Vertex shader): " << error_message << std::endl;
    }


    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDetachShader(shaderProgram1, vertexShader1);
    glDetachShader(shaderProgram1, fragmentShader1);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader1);
    glDeleteShader(fragmentShader1);

    //Green rect
    GLfloat vertices[] = {
       -0.8f,  0.6f,  0.0f, 0.0f, 1.0f, 0.0f, //triangle 1 vertex 1
        0.8f, -0.6f,  0.0f, 0.0f, 1.0f, 0.0f, //triangle 1 vertex 2
       -0.8f, -0.6f,  0.0f, 0.0f, 1.0f, 0.0f, //triangle 1 vertex 3
        0.8f,  0.6f,  0.0f, 0.0f, 1.0f, 0.0f  //triangle 2 vertex 2

    };
    GLuint indices[] = {
        0, 1, 2,
        0, 3, 1
    };

    //Yellow rect
    GLfloat vertices1[] = {
   -0.9f, -0.1f,  0.0f, //triangle 1 vertex 1
   -0.1f, -0.9f,  0.0f, //triangle 1 vertex 2
   -0.9f, -0.9f,  0.0f, //triangle 1 vertex 3
   -0.1f, -0.1f,  0.0f, //triangle 2 vertex 2

    };
    GLuint indices1[] = {
        0, 1, 2,
        0, 3, 1
    };



    // Vertex array object - VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //vertex buffer object VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //element buffer object EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Coordinates conf.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    //Color conf.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Vertex array object - VAO
    GLuint VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

    //vertex buffer object VBO
    GLuint VBO1;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    //element buffer object EBO
    GLuint EBO1;
    glGenBuffers(1, &EBO1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

    // Coordinates conf.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);





    glViewport(0, 0, (GLuint)window_width, (GLuint)window_height);

    // pêtla zdarzeñ
    while (!glfwWindowShouldClose(window))
    {
        // renderowanie
        glClearColor(0.298f, 0.141f, 0.141f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &EBO1);
    glDeleteProgram(shaderProgram1);


    glfwTerminate();
    return 0;
}