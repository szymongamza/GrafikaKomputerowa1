#include <glad/glad.h>  // musi byæ do³¹czony jako pierwszy
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>



const GLuint numberOfTriangles = 8;

const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"void main() {\n"
"    gl_Position = vec4(position, 1.0); // vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource =

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




    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, error_message);
        std::cout << "Error (Vertex shader): " << error_message << std::endl;
    }



    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);



    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, error_message);
        std::cout << "Error (Vertex shader): " << error_message << std::endl;
    }


    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Green rect
    const GLint numberOfVert = numberOfTriangles + 2;
    GLfloat twoTimesPI = 2.0f * 3.1415f;


    GLfloat radius = 0.5;


    GLfloat allVertTogether[numberOfVert * 3];

    allVertTogether[0] = 0;
    allVertTogether[1] = 0;
    allVertTogether[2] = 0;

    for (int i = 1; i < numberOfVert; i++) {
        allVertTogether[i * 3] = allVertTogether[0] + (radius * cos(i * twoTimesPI / numberOfTriangles));;
        allVertTogether[(i * 3)+1] = allVertTogether[0] + (radius * sin(i * twoTimesPI / numberOfTriangles));
        allVertTogether[(i * 3)+2] = allVertTogether[0];

    }


    //INDICIES
    GLuint circleIndicies[numberOfTriangles*3];


    for (int i = 0; i < numberOfTriangles; i++) {
        circleIndicies[i * 3] = 0;
        circleIndicies[(i * 3) + 1] = i + 1;
        circleIndicies[(i * 3) + 2] = i + 2;
    }


    // Vertex array object - VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //vertex buffer object VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(allVertTogether), allVertTogether, GL_STATIC_DRAW);

    //element buffer object EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(circleIndicies), circleIndicies, GL_STATIC_DRAW);
    
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
        glDrawElements(GL_TRIANGLES, numberOfTriangles * 3, GL_UNSIGNED_INT, 0);
        

        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);


    glfwTerminate();
    return 0;
}
