#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1920, 1080, "Camera triangles", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // White background
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // Tutorial 2: creating triangle
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint first_programID = LoadShaders("vertex1.glsl", "fragment1.glsl");
    GLuint second_programID = LoadShaders("vertex1.glsl", "fragment2.glsl");

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);

    glm::mat4 Model = glm::mat4(1.0f);

    GLuint first_MatrixID = glGetUniformLocation(first_programID, "MVP");
    GLuint second_MatrixID = glGetUniformLocation(second_programID, "MVP");

    static const GLfloat g_vertex_buffer_data[] = {
            -0.44f, 0.75f, 0.0f,
            -0.62f, -0.81f, 0.0f,
            0.32f, -0.04f, 0.0f,
            0.69f, 0.74f, 0.0f,
            -0.37f, -0.06f, 0.0f,
            0.13f, 0.48f, 0.0f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    float argument = 0.0;
    float distance = 3.0;
    float h_argument = 0.0;

    do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float x = distance * sin(argument);
        float z = distance * cos(argument);
        float y = distance * sin(h_argument);

        glm::mat4 View       = glm::lookAt(
                glm::vec3(x, 0, z), // Камера находится в мировых координатах (4,3,3)
                glm::vec3(0,0,0), // И направлена в начало координат
                glm::vec3(0,1,0)  // "Голова" находится сверху
        );

        glm::mat4 MVP = Projection * View * Model;

        // Draw triangle!!
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0, // smth for shaders
                3, // size
                GL_FLOAT, // Type of enum
                GL_FALSE, // Not normalized
                0, // (stride)
                (void*)0  // Array shift in buffer
        );

        // Use our shader
        glUseProgram(first_programID);
        // Draw the triangle !
        glUniformMatrix4fv(first_MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
        // Use our shader
        glUseProgram(second_programID);
        // Draw the triangle !
        glUniformMatrix4fv(second_MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 3, 3); // 3 indices starting at 0 -> 1 triangle

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        argument += 0.002f;
        h_argument += 0.0002f;
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(first_programID);
    glDeleteProgram(second_programID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

