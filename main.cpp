#include "Ship.h"
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, float x, float y);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float rotationAngle = 0.0f;
int attack_cd = 0;
std::vector<Ship*> ships;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform float rotation_angle;\n"
    "uniform float aspect_ratio;\n"
    "void main()\n"
    "{\n"
    "   mat4 rotation = mat4(cos(rotation_angle), -sin(rotation_angle), 0.0, 0.0,\n"
    "                        sin(rotation_angle), cos(rotation_angle), 0.0, 0.0,\n"
    "                        0.0,                 0.0,                  1.0, 0.0,\n"
    "                        0.0,                 0.0,                  0.0, 1.0);\n"
    "   gl_Position = rotation * vec4(aPos.x * aspect_ratio, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *shipShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 transform;

    void main() {
        gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";


const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.02f, -0.02f / (SCR_WIDTH / (float)SCR_HEIGHT),  // left  
         0.02f, -0.02f / (SCR_WIDTH / (float)SCR_HEIGHT),  // right 
         0.0f,   0.1f / (SCR_WIDTH / (float)SCR_HEIGHT)  // top   
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    ships.push_back(new Ship());
    unsigned int testB, testA;
    glGenVertexArrays(1, &testA);
    glGenBuffers(1, &testB);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(testA);

    glBindBuffer(GL_ARRAY_BUFFER, testB);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), ships[0]->getVertices(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        float cosAngle = cos(rotationAngle);
        float sinAngle = sin(rotationAngle);
        vertices[0] = -0.02f * cosAngle - (-0.02f / (SCR_WIDTH / (float)SCR_HEIGHT)) * sinAngle;
        vertices[1] = -0.02f * sinAngle + (-0.02f / (SCR_WIDTH / (float)SCR_HEIGHT)) * cosAngle;
        vertices[2] = 0.02f * cosAngle - (-0.02f / (SCR_WIDTH / (float)SCR_HEIGHT)) * sinAngle;
        vertices[3] = 0.02f * sinAngle + (-0.02f / (SCR_WIDTH / (float)SCR_HEIGHT)) * cosAngle;
        vertices[4] = 0.0f * cosAngle - (0.1f / (SCR_WIDTH / (float)SCR_HEIGHT)) * sinAngle;
        vertices[5] = 0.0f * sinAngle + (0.1f / (SCR_WIDTH / (float)SCR_HEIGHT)) * cosAngle;
        processInput(window, vertices[4], vertices[5]);
        attack_cd--;
        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        glUniform1f(glGetUniformLocation(shaderProgram, "rotation_angle"), rotationAngle);
        // Get the location of the aspect_ratio uniform
        int aspectRatioLocation = glGetUniformLocation(shaderProgram, "aspect_ratio");
        // Set the aspect ratio value
        glUniform1f(aspectRatioLocation, SCR_WIDTH / (float)SCR_HEIGHT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(ships[0]->getProgram());
        ships[0]->move();
        glGenVertexArrays(1, ships[0]->getVAO());
        glGenBuffers(1, ships[0]->getVBO());
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(*(ships[0]->getVAO()));

        glBindBuffer(GL_ARRAY_BUFFER, *(ships[0]->getVBO()));
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), ships[0]->getVertices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(*(ships[0]->getVAO()));

        glUniformMatrix4fv(glGetUniformLocation(ships[0]->getProgram(), "transform"), 1, GL_FALSE, glm::value_ptr(ships[0]->getTransform()));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        if (abs(ships[0]->getVertices()[0]) < 0.001f && abs(ships[0]->getVertices()[1]) < 0.001f ) {
            std::cout << "Game Over - Ship hit!" << std::endl;
            break; // End the game loop
        }
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    for(StraightLineObject* s: ships){
        delete s;
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, float x, float y)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Rotate left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rotationAngle -= 0.02f;

    // Rotate right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rotationAngle += 0.02f;

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        if(attack_cd > 0){
            return;
        }
        for(Ship* s: ships){
            attack_cd = 100;
            if(abs(x - ships[0]->getVertices()[0]) < 0.1f && abs(y - ships[0]->getVertices()[1]) < 0.1f){
                delete ships[0];
                ships[0] = new Ship();
            }
        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
