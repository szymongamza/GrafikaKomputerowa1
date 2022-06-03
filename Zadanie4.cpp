#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<cmath>


#define PI 3.14159265f

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 uColor = vec3(0.3f, 0.4f, 0.5f);\n"
"void main()\n"
"{\n"
"   FragColor = vec4(uColor, 1.0f);\n"
"}\n\0";

GLint numberOfTriangles;
GLuint shaderProgram;
GLfloat* vertices;
GLuint* indices;

GLuint VAO, VBO, EBO;



static GLfloat* createVertices(GLint numberOfTriangles, GLfloat radius) {
	GLfloat twoPi = 2.0f * PI;
	GLfloat* vertices = new GLfloat[(numberOfTriangles+2) * 3];

	vertices[0] = 0.0f;
	vertices[1] = 0.0f;
	vertices[2] = 0.0f;

	for (int i = 1; i < numberOfTriangles + 1; i++) {
		vertices[i * 3] = radius * cos(i * twoPi / numberOfTriangles);
		vertices[(i * 3) + 1] = radius * sin(i * twoPi / numberOfTriangles);
		vertices[(i * 3) + 2] = 0.0f;
	}

	return vertices;
}


GLuint* createIndices(GLint numberOfTriangles) {

	GLuint* indices = new GLuint[numberOfTriangles * 3];

	for (int i = 0; i < numberOfTriangles; i++) {
		indices[i * 3] = 0;
		indices[(i * 3) + 1] = i + 1;
		indices[(i * 3) + 2] = i + 2;
	}

	indices[(numberOfTriangles - 1) * 3 + 2] = 1;

	return indices;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

	if (yoffset > 0 && numberOfTriangles < 180) {
		numberOfTriangles++;
	}
	else if (yoffset < 0 && numberOfTriangles > 0) {
		numberOfTriangles--;
	}

	printf("Ilosc trojkatow: %d\n", numberOfTriangles);

	vertices = createVertices(numberOfTriangles, 0.75f);
	indices = createIndices(numberOfTriangles);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * (numberOfTriangles + 1), vertices, GL_DYNAMIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * numberOfTriangles, indices, GL_DYNAMIC_DRAW);

	glDrawElements(GL_TRIANGLES, numberOfTriangles * 3 + 2, GL_UNSIGNED_INT, 0);

}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	GLint uColorLocation = glGetUniformLocation(shaderProgram, "uColor");

	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		glUniform3f(uColorLocation, 1.0f, 0.0f, 0.0f);
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		glUniform3f(uColorLocation, 0.0f, 1.0f, 0.0f);
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		glUniform3f(uColorLocation, 0.0f, 0.0f, 1.0f);
	}
}

int main()
{
	// Initialize GLFW
	glfwInit();

	// OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// GLFW CORE profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels
	GLFWwindow* window = glfwCreateWindow(800, 800, "dynamic kolo", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);


	glfwSetScrollCallback(window, scrollCallback);


	glfwSetKeyCallback(window, keyboardCallback);



	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	//GLuint shaderProgram = glCreateProgram();
	shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	GLfloat radius = 0.75f;
	numberOfTriangles = 12;

	vertices = createVertices(numberOfTriangles, radius);

	indices = createIndices(numberOfTriangles);

	// Generate the VAO, VBO, and EBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * (numberOfTriangles + 1), vertices, GL_DYNAMIC_DRAW);

	// Bind the EBO specifying it's a GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Introduce the indices into the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * numberOfTriangles, indices, GL_DYNAMIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// Bind the EBO to 0
	// MAKE SURE TO UNBIND IT AFTER UNBINDING THE VAO, as the EBO is linked in the VAO
	// This does not apply to the VBO because the VBO is already linked to the VAO during glVertexAttribPointer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);
		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, numberOfTriangles * 3 + 2, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();

	}

	delete[] indices;

	delete[] vertices;

	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}