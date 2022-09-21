#include <cmath>

#include "./include/shader.h"
#include "./include//triangle.h"
#include "./include/open_gl_loader.h"
#include "./include/utils.h"

using namespace std;

void waiting_for_movement();

unsigned int VBOs[4], VAOs[4], EBOs[4];

Point center(0.0f, 0.0f, 0.0f);
Point left_point(-0.5f, 0.0f, 0.0f);
Triangle triangle(center, 0.02f);
Triangle triangle_2(left_point, 0.02f);

int main()
{
    Open_GL_Loader OpenGL(SCR_WIDTH, SCR_HEIGHT);

    Shader current_shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    objectSetterUp<Triangle>(triangle, VBOs[0], VAOs[0], EBOs[0]);
    objectSetterUp<Triangle>(triangle_2, VBOs[1], VAOs[1], EBOs[1]);

    while (!glfwWindowShouldClose(OpenGL.window))
    {
        waiting_for_movement();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        current_shader.use(); //Using previously compiled shaders
        
        glBindVertexArray(VAOs[0]);
        glDrawElements(mode, _size, GL_UNSIGNED_INT, 0);

        glBindVertexArray(VAOs[1]);
        glDrawElements(mode, _size, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(OpenGL.window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteBuffers(2, EBOs);
    
    return 0;
}

template <class ModifiedObject>
void update_object(ModifiedObject &currentObject, unsigned int _VBO, unsigned int _VAO)
{
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, currentObject.size() * sizeof(currentObject.get_vertices()), &currentObject.front(), GL_STATIC_DRAW);
}

void waiting_for_movement()
{
    if (!start) return;

    triangle.move_in_spiral(5.0f, -0.02f);
    triangle_2.move_in_circle(0.5f);

    update_object(triangle, VBOs[0], VAOs[0]);
    update_object(triangle_2, VBOs[1], VAOs[1]);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        glPointSize(8);
        _size = 3;
        mode = GL_POINTS;
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        _size = 3;
        mode = GL_TRIANGLES;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        start = true;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        start = false;
        triangle.stop();
        triangle_2.stop();
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}