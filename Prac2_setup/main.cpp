#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

bool moveRight = false;
bool moveLeft = false;
bool keyAPressed = false;
bool keyDPressed = false;

float carX = 0.0f; 
float carY = 0.0f;
float carSpeed = 0.0001f; 
float carMove = 0.001f;

float scaleX = 1.0f;
float scaleY = 1.0f;

const float laneChangeScale = 0.999f; 

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1000;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n" 
    "out vec3 OurColor;\n" 
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   OurColor = aColor;\n" 
    "}\0";


const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 OurColor;\n" 
    "void main()\n"
    "{\n"
    "   FragColor = vec4(OurColor, 1.0f);\n" 
    "}\n\0";


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Prac2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // Rectangle vertices (car body)
        -0.5f, -0.25f, 0.0f, 1.0f, 0.0f,  
         0.5f, -0.25f, 0.0f, 1.0f, 0.0f,  
         0.5f,  0.25f, 0.0f, 1.0f, 0.0f, 
        -0.5f,  0.25f, 0.0f, 1.0f, 0.0f, 
        // Square vertices (roof)
        -0.15f,  0.25f, 0.0f, 0.0f, 1.0f, 
         0.15f,  0.25f, 0.0f, 0.0f, 1.0f, 
         0.15f,  0.5f,  0.0f, 0.0f, 1.0f, 
        -0.15f,  0.5f,  0.0f, 0.0f, 1.0f,  
        // Small Square vertices (window)
        -0.08f,  0.3f, 0.0f, 1.0f, 1.0f, 
         0.08f,  0.3f, 0.0f, 1.0f, 1.0f, 
         0.08f,  0.45f,  0.0f, 1.0f, 1.0f, 
        -0.08f,  0.45f,  0.0f, 1.0f, 1.0f, 
        // Mirrored triangle vertices 
        0.505f,  0.25f, 0.0f, 0.0f, 1.0f, 
        0.15f,   0.25f, 0.0f, 0.0f, 1.0f, 
        0.15f,   0.50f,  0.0f, 0.0f, 1.0f,  
        // Smaller triangle vertices (window)
        0.32f,   0.32f, 1.0f, 1.0f, 1.0f, 
        0.2f,   0.32f, 1.0f, 1.0f, 1.0f, 
        0.2f,   0.42f, 1.0f, 1.0f, 1.0f,  
    };


    unsigned int indices[] = {
        // Rectangle indices (car body)
        0, 1, 2, 
        2, 3, 0,   
        // Square indices (roof)
        4, 5, 6,  
        6, 7, 4,   
        // Mirrored triangle indices
        12, 13, 14,  
        //small triangle
        15, 16, 17,
        // Small square indices
        8, 9, 10,  
        10, 11, 8,  
    };

//tyre
float wheelVertices[] = {
    -0.25f, -0.4f, 0.0f, 0.0f, 0.0f,
    -0.175f, -0.375f, 0.0f, 0.0f, 0.0f,
    -0.15f, -0.3f, 0.0f, 0.0f, 0.0f,
    -0.15f, -0.2f, 0.0f, 0.0f, 0.0f,
    -0.175f, -0.075f, 0.0f, 0.0f, 0.0f,
    -0.25f, -0.05f, 0.0f, 0.0f, 0.0f,
    -0.35f, -0.05f, 0.0f, 0.0f, 0.0f,
    -0.425f, -0.075f, 0.0f, 0.0f, 0.0f,
    -0.45f, -0.1f, 0.0f, 0.0f, 0.0f,
    -0.45f, -0.2f, 0.0f, 0.0f, 0.0f,
    -0.425f, -0.275f, 0.0f, 0.0f, 0.0f,
    -0.35f, -0.3f, 0.0f, 0.0f, 0.0f,
    -0.425f, -0.3f, 0.0f, 0.0f, 0.0f,

    0.35f, -0.4f, 0.0f, 0.0f, 0.0f,
    0.4f, -0.35f, 0.0f, 0.0f, 0.0f,
    0.425f, -0.3f, 0.0f, 0.0f, 0.0f,
    0.425f, -0.2f, 0.0f, 0.0f, 0.0f,
    0.4f, -0.15f, 0.0f, 0.0f, 0.0f,
    0.35f, -0.1f, 0.0f, 0.0f, 0.0f,
    0.3f, -0.075f, 0.0f, 0.0f, 0.0f,
    0.25f, -0.075f, 0.0f, 0.0f, 0.0f,
    0.2f, -0.1f, 0.0f, 0.0f, 0.0f,
    0.175f, -0.15f, 0.0f, 0.0f, 0.0f,
    0.15f, -0.2f, 0.0f, 0.0f, 0.0f,
    0.175f, -0.275f, 0.0f, 0.0f, 0.0f,
    0.25f, -0.3f, 0.0f, 0.0f, 0.0f,
    0.35f, -0.3f, 0.0f, 0.0f, 0.0f, 
};

    GLfloat backwheel[(52) * 3];

    GLfloat twoPi = 2.0f * M_PI;


    backwheel[0] = 0.0f;
    backwheel[1] = 0.0f;
    backwheel[2] = 0.0f;

    for (int i = 1; i < 50; i += 3)
    {
        float angle = (i * twoPi / 50);
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);

        backwheel[i * 3] = 0.0f + (0.1f * cosAngle);
        backwheel[(i * 3) + 1] = 0.0f + (0.1f * sinAngle);
        backwheel[(i * 3) + 2] = 0.0f;
    }    


    unsigned int wheelIndices[] = {
    0, 1, 11, 
    2, 3, 11, 
    3, 4, 11, 
    4, 5, 11,
    5, 6, 11, 
    6, 7, 11, 
    7, 8, 11, 
    8, 9, 11, 
    9, 10, 11,
    1, 2, 11,
    0, 10 ,11,

     //right wheel
    13, 14, 25,
    14, 15, 25,
    15, 16, 25,
    16, 17, 25,
    17, 18, 25,
    18, 19, 25,
    19, 20, 25,
    20, 21, 25,
    21, 22, 25,
    22, 23, 25,
    23, 24, 25,
    24, 13, 25,
};

// Vertices for the rims
float tireVertices[] = {
    // Circular outline
    0.2f, -0.4f, 0.0f, 0.0f, 0.0f,
    0.19509f, -0.39018f, 0.0f, 0.0f, 0.0f,
    0.18024f, -0.37281f, 0.0f, 0.0f, 0.0f,
    0.15643f, -0.34993f, 0.0f, 0.0f, 0.0f,
    0.125f, -0.32281f, 0.0f, 0.0f, 0.0f,
    0.09549f, -0.2939f, 0.0f, 0.0f, 0.0f,
    0.07071f, -0.26596f, 0.0f, 0.0f, 0.0f,
    0.05103f, -0.24095f, 0.0f, 0.0f, 0.0f,
    0.03662f, -0.22063f, 0.0f, 0.0f, 0.0f,
    0.02703f, -0.20611f, 0.0f, 0.0f, 0.0f,
    0.02131f, -0.19747f, 0.0f, 0.0f, 0.0f,
    0.01905f, -0.19411f, 0.0f, 0.0f, 0.0f,
    0.02131f, -0.19747f, 0.0f, 0.0f, 0.0f,
    0.02703f, -0.20611f, 0.0f, 0.0f, 0.0f,
    0.03662f, -0.22063f, 0.0f, 0.0f, 0.0f,
    0.05103f, -0.24095f, 0.0f, 0.0f, 0.0f,
    0.07071f, -0.26596f, 0.0f, 0.0f, 0.0f,
    0.09549f, -0.2939f, 0.0f, 0.0f, 0.0f,
    0.125f, -0.32281f, 0.0f, 0.0f, 0.0f,
    0.15643f, -0.34993f, 0.0f, 0.0f, 0.0f,
    0.18024f, -0.37281f, 0.0f, 0.0f, 0.0f,
    0.19509f, -0.39018f, 0.0f, 0.0f, 0.0f,
    0.2f, -0.4f, 0.0f, 0.0f, 0.0f,
    0.19509f, -0.39018f, 0.0f, 0.0f, 0.0f,
    0.19509f, -0.39018f, 0.0f, 0.0f, 0.0f,
    0.19509f, -0.39018f, 0.0f, 0.0f, 0.0f,
    0.1f, -0.2f, 0.0f, 0.0f, 0.0f,
};


unsigned int tireIndices[] = {
   // Triangles forming the circular outline
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 5,
    0, 5, 6,
    0, 6, 7,
    0, 7, 8,
    0, 8, 9,
    0, 9, 10,
    0, 10, 11,
    0, 11, 12,
    0, 12, 13,
    0, 13, 14,
    0, 14, 15,
    0, 15, 16,
    0, 16, 17,
    0, 17, 18,
    0, 18, 19,
    0, 19, 20,
    0, 20, 21,
    0, 21, 22,
    0, 22, 23,
    0, 23, 24,
    0, 24, 25,
    0, 25, 26,
    0, 26, 27,
    0, 27, 28,
    0, 28, 29,
    0, 29, 30,
    0, 30, 1,
    // Triangles forming the shell
    1, 2, 3,
    2, 3, 4,
    3, 4, 5,
    4, 5, 6,
    5, 6, 7,
    6, 7, 8,
    7, 8, 9,
    8, 9, 10,
    9, 10, 11,
    10, 11, 12,
    11, 12, 13,
    12, 13, 14,
    13, 14, 15,
    14, 15, 16,
    15, 16, 17,
    16, 17, 18,
    17, 18, 19,
    18, 19, 20,
    19, 20, 21,
    20, 21, 22,
    21, 22, 23,
    22, 23, 24,
    23, 24, 25,
    24, 25, 26,
    25, 26, 27,
    26, 27, 28,
    27, 28, 29,
    28, 29, 30,
    29, 30, 1
};


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);  

    //TYRES//
    unsigned int tireVAO, tireVBO, tireEBO;
    glGenVertexArrays(1, &tireVAO);
    glGenBuffers(1, &tireVBO);
    glGenBuffers(1, &tireEBO);

    glBindVertexArray(tireVAO);

    glBindBuffer(GL_ARRAY_BUFFER, tireVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tireVertices), tireVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tireEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tireIndices), tireIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //WHEELS
    unsigned int wheelVAO, wheelVBO, wheelEBO;
    glGenVertexArrays(1, &wheelVAO);
    glGenBuffers(1, &wheelVBO);
    glGenBuffers(1, &wheelEBO);

    glBindVertexArray(wheelVAO);

    glBindBuffer(GL_ARRAY_BUFFER, wheelVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wheelVertices), wheelVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wheelEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wheelIndices), wheelIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    bool wireframeMode = false;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            carX += carSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            carX -= carSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !keyAPressed)
        {
            carY += carMove;
            scaleX *= laneChangeScale; 
            scaleY *= laneChangeScale; 
            keyAPressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
        {
            keyAPressed = false;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !keyDPressed)
        {
            carY -= carMove;
            scaleX /= laneChangeScale; 
            scaleY /= laneChangeScale;
            keyDPressed = true;
        } 
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
        {
            keyDPressed = false;
        }
        
        // Check if the car is moving out of the visible area
        if (carX > 1.0f || carX < -1.0f || carY > 1.0f || carY < -1.0f) {
            carX = 0.0f;
            carY = 0.0f;
            scaleX = 1.0f;
            scaleY = 1.0f;
        }

        for (int i = 0; i < sizeof(vertices) / sizeof(float); i += 5)
        {
            vertices[i] *= scaleX;
            vertices[i + 1] *= scaleY;
        }

         for (int i = 0; i < sizeof(wheelVertices) / sizeof(float); i += 5)
        {
            wheelVertices[i] *= scaleX;
            wheelVertices[i + 1] *= scaleY;
        }

        for (int i = 0; i < sizeof(vertices) / sizeof(float); i += 5)
        {
            vertices[i] += carX;
            vertices[i+1] += carY;
        }

         for (int i = 0; i < sizeof(wheelVertices) / sizeof(float); i += 5)
        {
            wheelVertices[i] += carX;
            wheelVertices[i+1] += carY;
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, wheelVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(wheelVertices), wheelVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw shapes
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); 
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

        // Draw the wheels
        glBindVertexArray(wheelVAO);
        glDrawElements(GL_TRIANGLES, 80, GL_UNSIGNED_INT, 0);

        // Draw the rims
      //   glBindVertexArray(tireVAO);
      //   glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

        
        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteProgram(shaderProgram);

      //   glDeleteVertexArrays(1, &tireVAO);
      //   glDeleteBuffers(1, &tireVBO);
      //   glDeleteBuffers(1, &tireEBO);

        glDeleteVertexArrays(1, &wheelVAO);
        glDeleteBuffers(1, &wheelVBO);
        glDeleteBuffers(1, &wheelEBO);

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
        return 0;
    }


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
