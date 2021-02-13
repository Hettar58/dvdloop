
//  main.cpp
//  openGL
//
//  Created by Yann Trou on 31/03/2019.
//  Copyright © 2019 com.generic. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>
#include <GLUT/GLUT.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>//Pour utiliser les matrices de mise a l'échelle
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <time.h>
#include "common/stb_image.hpp"
using namespace glm;
#include "common/Shader.hpp"
#include "common/Texture.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

double frand_a_b(double a, double b) {
    return (rand() / (double)RAND_MAX) * (b - a) + a;
}


int main() {
    //Test de l'initialisation de GLFW
    if (!glfwInit()) {
        fprintf(stderr, "impossible d'initialiser GLFW\n");
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4); //antialiasing x4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //on veut openGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //on veut openGL 3.3
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //compatibilité macOS
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //utilise uniquement la version choisie d'oepnGL
    glfwSwapInterval(1);
    
    //Pointeur d'accès a la fenêtre
    GLFWwindow *window;
    //création de la fenêtre en 800 par 600
    window = glfwCreateWindow(800, 600, "DVD Loop", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //Teste la création de la fenêtre
    if (window == NULL) {
        fprintf(stderr, "impossible de créer la fenêtre GLFW");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window); //initialise GLEW
    glewExperimental = true; //nécessaire dans le profil de base
                             //Teste l'initialisation de GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "impossible d'initialiser GLEW");
        glfwTerminate();
        return -1;
    }
    //Fait en sorte que l'on puisse récupérer les appuis de touches.s
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //FIN INIT -------------------------------------------------------------------------------------------------------------------------------------------------------------------
    
    
    float vertices[] = {
        -0.75f, 0.5f, 0.0f,    0.0f, 1.0f, //haut gauche
        0.75f, 0.5f, 0.0f,    1.0f, 1.0f, //haut droite
        0.75f, -0.5f, 0.0f,    1.0f, 0.0f, //bas droite
        -0.75f, -0.5f, 0.0f,    0.0f, 0.0f //bas gauche
    };
    
    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    GLuint shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    
    unsigned int texture1 = loadTexture("logo_dvd.png");
    int textureLocation = glGetUniformLocation(shaderID, "tex");
    glUniform1i(textureLocation, texture1);
    
    bool close = false;
    double x = 0, vx = 1;
    double y = 0, vy = 1;
    float r = 1;
    float g = 1;
    float b = 1;
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    do {
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
                                             // printf and reset timer
            char title[256];
            title[255] = '\0';
            
            snprintf(title, 255,
                     "%s %s - [MPF: %3.2f] - [FPS: %i]",
                     "DVD Loop", "1.0", 1000.0f / (float)nbFrames, nbFrames);
            
            glfwSetWindowTitle(window, title);
            
            nbFrames = 0;
            lastTime += 1.0;
        }
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUseProgram(shaderID);
        
        //pour que le logo fasse un cercle
        //float timeValue = glfwGetTime();
        //double posX = sin(timeValue) * 3.25;
        //double posY = cos(timeValue) * 3.5;
        
        
        x += vx * 0.016;
        y += vy * 0.016;
        if (x >= 3.3) {
            vx = -1 + -frand_a_b(0, 1);;
            r = frand_a_b(0, 1);
            g = frand_a_b(0, 1);
            b = frand_a_b(0, 1);
        }
        if (x <= -3.3) {
            vx = 1 + frand_a_b(0, 1);
            r = frand_a_b(0, 1);
            g = frand_a_b(0, 1);
            b = frand_a_b(0, 1);
        }
        if (y >= 3.5) {
            vy = -1 + -frand_a_b(0, 1);
            r = frand_a_b(0, 1);
            g = frand_a_b(0, 1);
            b = frand_a_b(0, 1);
        }
        if (y <= -3.5) {
            vy = 1 + frand_a_b(0, 1);
            r = frand_a_b(0, 1);
            g = frand_a_b(0, 1);
            b = frand_a_b(0, 1);
        }
        
        
        unsigned int colorLoc = glGetUniformLocation(shaderID, "col");
        glUniform3f(colorLoc, r, g, b);
        
        mat4 transformationMatrix = mat4(1.0f);
        transformationMatrix = scale(transformationMatrix, vec3(0.25, 0.25, 0.25));
        transformationMatrix = translate(transformationMatrix, vec3(x, y, 0.0));
        unsigned int transformLoc = glGetUniformLocation(shaderID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transformationMatrix));
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window) != 0) {
            close = true;
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
    } while (close == false);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderID);
    glfwTerminate();
    return 0;
}
