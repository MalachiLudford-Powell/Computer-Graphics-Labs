#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common/shader.hpp>
#include <common/texture.hpp>

// Function prototypes
void keyboardInput(GLFWwindow *window);

int main( void )
{
    // =========================================================================
    // Window creation - you shouldn't need to change this code
    // -------------------------------------------------------------------------
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Lab03 Textures", NULL, NULL);
    
	if( window == NULL ){
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
    glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}    
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================
    
	// Ensure we can capture keyboard inputs
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    ////creates 1 triangle 
    //// Define vertices
    //const float vertices[] = {
    //    // x     y     z
    //    -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f,  0.5f, 0.0f
    //};

    //// define texture coordinates 
    //const float uv[] = {
    //    // u   v
    //    0.0f, 0.0f,
    //    1.0f, 0.0f,
    //    0.5f, 1.0f
    //};

    //// creates 2 triangles to make a square 
    //// Define vertex positions
    //static const float vertices[] = {
    //    // x     y     z
    //    -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.5f,  0.5f, 0.0f,
    //    -0.5f, -0.5f, 0.0f,
    //     0.5f,  0.5f, 0.0f,
    //    -0.5f,  0.5f, 0.0f
    //};

    //// Define texture coordinates
    //static const float uv[] = {
    //    // u    v
    //    0.0f,  0.0f,    // triangle 1
    //    1.0f,  0.0f,
    //    1.0f,  1.0f,
    //    0.0f,  0.0f,    // triangle 2
    //    1.0f,  1.0f,
    //    0.0f,  1.0f
    //};

    // Define vertex positions
    static const float vertices[] = {
        // x     y     z      index
        -0.5f, -0.5f, 0.0f,  // 0       3 -- 2
         0.5f, -0.5f, 0.0f,  // 1       |  / |  
         0.5f,  0.5f, 0.0f,  // 2       | /  |
        -0.5f,  0.5f, 0.0f   // 3       0 -- 1
    };

    // Define texture coordinates
    static const float uv[] = {
        // u    v      index
        0.0f,  0.0f,  // 0
        1.0f,  0.0f,  // 1
        1.0f,  1.0f,  // 2
        0.0f,  1.0f,  // 3
    };

    // Define indices
    static const unsigned int indices[] = {
        0, 1, 2,  // lower-right triangle
        0, 2, 3   // upper-left triangle
    };

  
    // Create the Vertex Array Object (VAO)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create Vertex Buffer Object (VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create Element Buffer Object (EBO)
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // create texture buffer 
    unsigned int uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
    
    // Compile shader program
    unsigned int shaderID;
    shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    
    // Use the shader program
    glUseProgram(shaderID);
    
    // create a bind texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Load texture image from file
    const char* path = "../assets/crate.jpg"; // uses pointer to find saved texture on computer ..?
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);

    if (data)
        std::cout << "Texture loaded." << std::endl;
    else
        std::cout << "Texture not loaded. Check the path." << std::endl;

    // specify 2D texture 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // only for jpeg
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the image from the memory
    stbi_image_free(data);

    // Render loop
	while (!glfwWindowShouldClose(window))
    {
        // Get inputs
        keyboardInput(window);
        
        // Clear the window
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Send the VBO to the shaders
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // send the uv buffer to the shaders 
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        // Bind the texture to the VAO
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);

        //// Draw the triangle
        //glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));
        //glDisableVertexAttribArray(0);
        // 
        // Draw the triangles
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    
    // Cleanup
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderID);
    
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

void keyboardInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
