#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLT_IMPLEMENTATION
#include "gltext.h" /* https://github.com/vallentin/glText */

#include "objfunc/obj.hpp"
#include "simulated_annealing/sa.hpp" /* Simulated Annealing */
#include "steepest_ascent_hl/sahl.hpp" /* Steepest Ascent Hill Climbing*/
#include "sideway_move/sm.hpp" /* Hill Climbing Sideway Move*/
#include "stochastic_hc/shc.hpp" /* Stochastic Hill Climbing*/
#include "genetic/ga.hpp" /* Genetic Algortihm */

const int SCR_WIDTH = 1200;
const int SCR_HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float pitch = 0.0f, yaw = -90.0f;
float sensitivity = 0.2f;
bool firstMouse = true;
bool isMouseButtonPressed = false;
std::vector<std::vector<std::vector<GLTtext*>>> texts(5, std::vector<std::vector<GLTtext*>>(5, std::vector<GLTtext*>(5, nullptr)));
GLTtext* text_continue;
GLTtext* title;

std::vector<std::vector<std::vector<int>>> perfectMagicCube = {
    {
        {25, 16, 80, 104, 90},
        {115, 98, 4, 1, 97},
        {42, 111, 85, 2, 75},
        {66, 72, 27, 102, 48},
        {67, 18, 119, 106, 5}
    },
    {
        {91, 77, 71, 6, 70},
        {52, 64, 117, 69, 13},
        {30, 118, 21, 123, 23},
        {26, 39, 92, 44, 114},
        {116, 17, 14, 73, 95}
    },
    {
        {47, 61, 45, 76, 86},
        {107, 43, 38, 33, 94},
        {89, 68, 63, 58, 37},
        {32, 93, 88, 83, 19},
        {40, 50, 81, 65, 79}
    },
    {
        {31, 53, 112, 109, 10},
        {12, 82, 34, 87, 100},
        {103, 3, 105, 8, 96},
        {113, 57, 9, 62, 74},
        {56, 120, 55, 49, 35}
    },
    {
        {121, 108, 7, 20, 59},
        {29, 28, 122, 125, 11},
        {51, 15, 41, 124, 84},
        {78, 54, 99, 24, 60},
        {36, 110, 46, 22, 101}
    }
};

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transform;
void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 0.2);
}
)";

float vertices[] = {
    -0.4f, -0.4f, -0.4f,
    0.4f, -0.4f, -0.4f,
    0.4f, 0.4f, -0.4f,
    -0.4f, 0.4f, -0.4f,
    -0.4f, -0.4f, 0.4f,
    0.4f, -0.4f, 0.4f,
    0.4f, 0.4f, 0.4f,
    -0.4f, 0.4f, 0.4f
};

unsigned int indices[] = {
    0, 1, 1, 2, 2, 3, 3, 0,
    4, 5, 5, 6, 6, 7, 7, 4,
    0, 4, 1, 5, 2, 6, 3, 7
};

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (!isMouseButtonPressed)
    {
        firstMouse = true;
        return;
    }

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
            isMouseButtonPressed = true;
        else if (action == GLFW_RELEASE)
            isMouseButtonPressed = false;
    }
}

int displayState(std::vector<std::vector<std::vector<int>>> cube, bool isInitial){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Magic Cube", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!gltInit())
    {
        std::cout << "Failed to initialize glText" << std::endl;
        glfwTerminate();
        return -1;
    }
    gltBeginDraw();
     for (int i = 0; i < 5; ++i)
        for(int j = 0; j < 5; j++)
            for(int k = 0; k < 5; k++)
                texts[i][j][k] = gltCreateText();

    for (int i = 0; i < 5; ++i)
        for(int j = 0; j < 5; j++)
            for(int k = 0; k < 5; k++)
                gltSetText(texts[i][j][k], std::to_string(cube[i][j][k]).c_str());
    
    while (!glfwWindowShouldClose(window)){
   
    processInput(window);

    // Clear screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // PRojection matrix
    glUseProgram(shaderProgram);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
    view = glm::rotate(view, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

    for (int x = -2; x <= 2; ++x) {
            for (int y = -2; y <= 2; ++y) {
                for (int z = -2; z <= 2; ++z) {
                    //int i = x + 2, j = y + 2, k = z + 2;
                    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
                    glm::mat4 transform = projection * view * model;
                    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
                    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
                    glBindVertexArray(VAO);
                    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
                }
            }
        }

        // Text rendering for cube values
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                for (int k = 0; k < 5; ++k) {
                    if (texts[i][j][k]) {
                        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i - 2, j - 2, k - 2));
                        glm::mat4 transform = projection * view * model;
                        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
                        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
                        glBindVertexArray(VAO);
                        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

                        glm::vec4 textPosition = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
                        if (textPosition.w > 0.0f) {
                            textPosition /= textPosition.w;
                            int xpos = (int)((textPosition.x * 0.5f + 0.5f) * SCR_WIDTH);
                            int ypos = (int)((-textPosition.y * 0.5f + 0.5f) * SCR_HEIGHT);

                            gltBeginDraw();
                                    bool isPartOfMagicSum = (
                                isMagicRow(cube, i, j) || 
                                isMagicColumn(cube, i, k) || 
                                isMagicDiagonal(cube, i) || 
                                isMagicPillar(cube, j, k) || 
                                isMagic3DDiagonal(cube)
                            );

                            if (isPartOfMagicSum) {
                                int constraintCount = countConstraints(cube, i, j, k);
                                if(constraintCount < 2){
                                    gltColor(0.0f, 1.0f, 0.0f, 1.0f);  // Ijo
                                } else if (constraintCount >= 2 && constraintCount < 5)
                                {
                                    gltColor(0.3f, 0.7f, 0.0f, 1.0f);  // Ijo
                                } else if(constraintCount >= 5 && constraintCount < 8) {
                                    gltColor(0.5f, 0.5f, 0.0f, 1.0f);  // Ijo
                                } else if(constraintCount >= 8 && constraintCount < 11) {
                                    gltColor(0.7f, 0.3f, 0.0f, 1.0f);  // Ijo
                                } else if(constraintCount >= 11) {
                                    gltColor(0.0f, 0.2f, 0.0f, 1.0f);  // Ijo
                                }
                            } else {
                                gltColor(1.0f, 0.0f, 0.0f, 1.0f);  // Merah
                            }
                            gltDrawText2D(texts[i][j][k], xpos, ypos, 1.0f);
                            gltEndDraw();
                        }
                    }
                }
            }
        }

        gltBeginDraw();
        text_continue = gltCreateText();
        title = gltCreateText();
        if(isInitial){
            gltSetText(title, "INITIAL STATE");
        } else {
            gltSetText(title, "FINAL STATE");
        }
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D(title, (SCR_WIDTH / 2) - 100, 20, 2.0f);
        gltEndDraw();
        gltBeginDraw();
        text_continue = gltCreateText();
        gltSetText(text_continue, "PRESS SPACEBAR TO CONTINUE");
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D(text_continue, (SCR_WIDTH / 2) - 200, 750, 2.0f);
        gltEndDraw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            for (int k = 0; k < 5; ++k) {
            if (texts[i][j][k]) {
                gltDeleteText(texts[i][j][k]);
                }
            }
        }
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    gltTerminate();
    glfwTerminate();
    return 0;
}


    

int main(int argc, char *argv[])
{
    int algoritma = -1;
    std::string namaalgoritma = "";
    do {
        std::cout << "=================MAGIC NUMBER CUBE SIMULATION====================" << std::endl;
        std::cout << "Algoritma apa yang diinginkan?" << std::endl;
        std::cout << "(1) Perfect Magic Cube Example" << std::endl;
        std::cout << "(2) Steepest Ascent Hill Climbing" << std::endl;
        std::cout << "(3) Hill Climbing with Sideway Move" << std::endl;
        std::cout << "(4) Random Restart Hill Climbing" << std::endl;
        std::cout << "(5) Stochastic Hill Climbing" << std::endl;
        std::cout << "(6) Simulated Annealing" << std::endl;
        std::cout << "(7) Genetic Algorithm" << std::endl;
        std::cout << "(8) EXIT" << std::endl;
        std::cout << "================================================================" << std::endl;
        std::cin >> algoritma;
        if(algoritma < 1 && algoritma > 8 ){
            std::cout << "INPUT TIDAK VALID!" << std::endl;
        }else if(algoritma == 8){
            exit(0);
        } else if (algoritma == 1){
            displayState(perfectMagicCube, false);
        } else {
            std::cout << "Loading Initial State..." << std::endl;
        std::vector<std::vector<std::vector<int>>> initial_cube = initialize_random_cube();
        displayState(initial_cube, true);
        Result result;
        switch(algoritma){
            case 2:
                result = steepest_ascent_hill_climbing(initial_cube);
                namaalgoritma = "STEEPEST ASCENT HILL CLIMBING";
                break;
            case 3:
                int sidewayMoves;
                std::cout << "Masukkan jumlah maksimal sideway moves: ";
                std::cin >> sidewayMoves;
                result = hill_climbing_with_sideway_moves(initial_cube, sidewayMoves);
                namaalgoritma = "HILL CLIMBING WITH SIDEWAY MOVES";
                break;
            case 5: 
                int stochasticMoves;
                std::cout << "Masukkan jumlah maksimal iterasi untuk stochastic hill climbing: ";
                std::cin >> stochasticMoves;
                result = stochastic_hill_climbing(initial_cube, stochasticMoves);
                namaalgoritma = "STOCHASTIC HILL CLIMBING";
                break;
            case 6:
                result = simulated_annealing(initial_cube);
                namaalgoritma = "SIMULATED ANNEALING";
                break;
            case 7:
                int N = 5; 
                int ukuranPopulasi = 100;
                int maxGenerasi = 10000; 
                double tingkatCrossover = 0.8;
                double tingkatMutasi = 0.05;   
                result = algoritmaGenetik(N, ukuranPopulasi, maxGenerasi, tingkatCrossover, tingkatMutasi);
                namaalgoritma = "GENETIC ALGORITHM";
                break;
            //tambahin case masing masing lagi
        }
        std::cout << "=================LOCAL SEARCH REPORT====================" << std::endl;
        std::cout << "SEARCH ALGORITHM  :" << namaalgoritma << std::endl;
        std::cout << "FINAL ERROR       : " << result.error << std::endl;
        std::cout << "TIME ESTIMATED    : " << result.time_taken << " SECONDS" << std::endl;
        std::cout << "STEPS TAKEN       : " << result.steps << std::endl;
        std::cout << "========================================================" << std::endl;
        std::cout << std::endl;
        displayState(result.cube, false);
        }
        
    }
    while(true);
    return 0;
}
