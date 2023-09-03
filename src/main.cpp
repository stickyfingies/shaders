#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

static constexpr const unsigned short GL_MAJOR = 4;
static constexpr const unsigned short GL_MINOR = 3;

static constexpr const unsigned short WIN_WIDTH = 800;
static constexpr const unsigned short WIN_HEIGHT = 600;

float vertices[] = {
     1.f,  1.f, 0.0f,  // top right
     1.f, -1.f, 0.0f,  // bottom right
    -1.f, -1.f, 0.0f,  // bottom left
    -1.f,  1.f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

using BufferLayout = unsigned int;
using Buffer = unsigned int;
using Shader = unsigned int;
using Program = unsigned int;

void on_framebuffer_resize(GLFWwindow *window, int width, int height);
void poll_input(GLFWwindow *window);
Shader create_shader(const char *fileName, unsigned int shaderType);
void query_gl_stats();

int main()
{
    using namespace std;

    // Window

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Shaders", nullptr, nullptr);
    if (window == nullptr)
    {
        cerr << "Failed to create GLFW window." << endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    if (gl3wInit() != 0)
    {
        cerr << "Failed to load OpenGL bindings" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // OpenGL

    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, on_framebuffer_resize);
    query_gl_stats();

    BufferLayout VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    Buffer VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    Buffer EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    const Shader vertexShader = create_shader("./shaders/vertex.glsl", GL_VERTEX_SHADER);
    const Shader fragmentShader = create_shader("./shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    const Program shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cerr << infoLog << endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glBindVertexArray(VAO);
    glUseProgram(shaderProgram);

    // Loop

    while (!glfwWindowShouldClose(window))
    {
        poll_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        int w, h;
        glfwGetWindowSize(window, &w, &h);
        glUniform1f(0, glfwGetTime());
        glUniform2i(1, w, h);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // End

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void query_gl_stats() {
    using namespace std;

    cout << "OpenGL " << GL_MAJOR << "." << GL_MINOR << endl;

    int maxVertexAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
    cout << "Vertex attributes: MAX=" << maxVertexAttribs << endl;
}

Shader create_shader(const char *fileName, unsigned int shaderType)
{
    using namespace std;

    // Load
    string shaderContent;
    getline(ifstream(fileName), shaderContent, '\0');
    const char *shaderSource = shaderContent.c_str();

    // Compile
    Shader shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Failure
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cerr << infoLog << endl;
    }

    return shader;
}

void poll_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void on_framebuffer_resize(GLFWwindow *window, int width, int height)
{
    std::cout << "Resized to " << width << "x" << height << std::endl;
    glViewport(0, 0, width, height);
}