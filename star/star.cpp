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
    window = glfwCreateWindow( 1920, 1080, "Playground", NULL, NULL);
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

    // White background
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Tutorial 2: creating triangle
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("vertexshader.glsl", "fragmentshader.glsl");

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices

    const float PI = 3.1415;

    float ext_vertex[5][2] = { 0 };
    float int_vertex[5][2] = { 0 };

    float arg = 2 * PI + PI / 2;

    for(int i = 0; i < 5; ++i) {
        ext_vertex[i][0] = cos(arg);
        ext_vertex[i][1] = sin(arg);

        int_vertex[i][0] = 0.45f * cos(arg + PI);
        int_vertex[i][1] = 0.45f * sin(arg + PI);

        arg -= 2 * PI / 5;
    };

    float front_z = 0.2f;

    // 0 <-> x; 1 <-> y
    static const GLfloat g_vertex_buffer_data[] = {
            0.0f, 0.0f, front_z,
            ext_vertex[0][0], ext_vertex[0][1], 0.0f,
            int_vertex[3][0], int_vertex[3][1], 0.0f,

            0.0f, 0.0f, front_z,
            int_vertex[3][0], int_vertex[3][1], 0.0f,
            ext_vertex[1][0], ext_vertex[1][1], 0.0f,

            0.0f, 0.0f, front_z,
            ext_vertex[1][0], ext_vertex[1][1], 0.0f,
            int_vertex[4][0], int_vertex[4][1], 0.0f,

            0.0f, 0.0f, front_z,
            int_vertex[4][0], int_vertex[4][1], 0.0f,
            ext_vertex[2][0], ext_vertex[2][1], 0.0f,

            0.0f, 0.0f, front_z,
            ext_vertex[2][0], ext_vertex[2][1], 0.0f,
            int_vertex[0][0], int_vertex[0][1], 0.0f,

            0.0f, 0.0f, front_z,
            int_vertex[0][0], int_vertex[0][1], 0.0f,
            ext_vertex[3][0], ext_vertex[3][1], 0.0f,

            0.0f, 0.0f, front_z,
            ext_vertex[3][0], ext_vertex[3][1], 0.0f,
            int_vertex[1][0], int_vertex[1][1], 0.0f,

            0.0f, 0.0f, front_z,
            int_vertex[1][0], int_vertex[1][1], 0.0f,
            ext_vertex[4][0], ext_vertex[4][1], 0.0f,

            0.0f, 0.0f, front_z,
            ext_vertex[4][0], ext_vertex[4][1], 0.0f,
            int_vertex[2][0], int_vertex[2][1], 0.0f,

            0.0f, 0.0f, front_z,
            int_vertex[2][0], int_vertex[2][1], 0.0f,
            ext_vertex[0][0], ext_vertex[0][1], 0.0f,


            // Backside
            0.0f, 0.0f, -front_z,
            ext_vertex[0][0], ext_vertex[0][1], 0.0f,
            int_vertex[3][0], int_vertex[3][1], 0.0f,

            0.0f, 0.0f, -front_z,
            int_vertex[3][0], int_vertex[3][1], 0.0f,
            ext_vertex[1][0], ext_vertex[1][1], 0.0f,

            0.0f, 0.0f, -front_z,
            ext_vertex[1][0], ext_vertex[1][1], 0.0f,
            int_vertex[4][0], int_vertex[4][1], 0.0f,

            0.0f, 0.0f, -front_z,
            int_vertex[4][0], int_vertex[4][1], 0.0f,
            ext_vertex[2][0], ext_vertex[2][1], 0.0f,

            0.0f, 0.0f, -front_z,
            ext_vertex[2][0], ext_vertex[2][1], 0.0f,
            int_vertex[0][0], int_vertex[0][1], 0.0f,

            0.0f, 0.0f, -front_z,
            int_vertex[0][0], int_vertex[0][1], 0.0f,
            ext_vertex[3][0], ext_vertex[3][1], 0.0f,

            0.0f, 0.0f, -front_z,
            ext_vertex[3][0], ext_vertex[3][1], 0.0f,
            int_vertex[1][0], int_vertex[1][1], 0.0f,

            0.0f, 0.0f, -front_z,
            int_vertex[1][0], int_vertex[1][1], 0.0f,
            ext_vertex[4][0], ext_vertex[4][1], 0.0f,

            0.0f, 0.0f, -front_z,
            ext_vertex[4][0], ext_vertex[4][1], 0.0f,
            int_vertex[2][0], int_vertex[2][1], 0.0f,

            0.0f, 0.0f, -front_z,
            int_vertex[2][0], int_vertex[2][1], 0.0f,
            ext_vertex[0][0], ext_vertex[0][1], 0.0f,
    };

    static const GLfloat g_color_buffer_data[] = {
            0.831f,  0.680f,  0.839f,
            0.831f,  0.380f,  0.639f,
            0.831f,  0.530f,  0.739f,

            0.701f,  0.183f,  0.564f,
            0.551f,  0.083f,  0.364f,
            0.351f,  0.0f,  0.264f,

            0.851f,  0.333f,  0.764f,
            0.651f,  0.133f,  0.564f,
            0.751f,  0.233f,  0.664f,

            0.614f,  0.438f,  0.892f,
            0.414f,  0.250f,  0.700f,
            0.314f,  0.100f,  0.550f,

            0.464f,  0.288f,  0.692f,
            0.314f,  0.188f,  0.592f,
            0.100f,  0.188f,  0.192f,

            0.329f,  0.462f,  0.815f,
            0.229f,  0.362f,  0.715f,
            0.129f,  0.262f,  0.615f,

            0.229f,  0.352f,  0.565f,
            0.129f,  0.212f,  0.415f,
            0.229f,  0.102f,  0.315f,

            0.211f,  0.611f,  0.905f,
            0.111f,  0.511f,  0.805f,
            0.011f,  0.411f,  0.705f,

            0.0f,  0.261f,  0.555f,
            0.011f,  0.391f,  0.675f,
            0.0f,  0.101f,  0.355f,

            0.851f,  0.400f,  0.659f,
            0.831f,  0.280f,  0.539f,
            0.831f,  0.180f,  0.439f,


            // backside
            0.851f,  0.400f,  0.659f,
            0.831f,  0.280f,  0.539f,
            0.831f,  0.180f,  0.439f,

            0.851f,  0.333f,  0.764f,
            0.651f,  0.133f,  0.564f,
            0.751f,  0.233f,  0.664f,

            0.701f,  0.183f,  0.564f,
            0.551f,  0.083f,  0.364f,
            0.351f,  0.0f,  0.264f,

            0.464f,  0.288f,  0.692f,
            0.314f,  0.188f,  0.592f,
            0.100f,  0.188f,  0.192f,

            0.614f,  0.438f,  0.892f,
            0.414f,  0.250f,  0.700f,
            0.314f,  0.100f,  0.550f,

            0.229f,  0.352f,  0.565f,
            0.129f,  0.212f,  0.415f,
            0.229f,  0.102f,  0.315f,

            0.329f,  0.462f,  0.815f,
            0.229f,  0.362f,  0.715f,
            0.129f,  0.262f,  0.615f,

            0.0f,  0.261f,  0.555f,
            0.011f,  0.391f,  0.675f,
            0.0f,  0.101f,  0.355f,

            0.211f,  0.611f,  0.905f,
            0.111f,  0.511f,  0.805f,
            0.011f,  0.411f,  0.705f,

            0.831f,  0.680f,  0.839f,
            0.831f,  0.380f,  0.639f,
            0.831f,  0.530f,  0.739f,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    float argument = 0.0;

    do{

        float x = 7.0f * sin(argument);
        float z = 7.0f * cos(argument);

        // Camera matrix
        glm::mat4 View       = glm::lookAt(
                glm::vec3(x,0,z), //
                glm::vec3(0,0,0), // looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        // Color triangles
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                1,                                // Атрибут. Здесь необязательно указывать 1, но главное, чтобы это значение совпадало с layout в шейдере..
                3,                                // Размер
                GL_FLOAT,                         // Тип
                GL_FALSE,                         // Нормализован?
                0,                                // Шаг
                (void*)0                          // Смещение
        );

        glUseProgram(programID);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 2 * 5);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        argument += 0.002f;

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

