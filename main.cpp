#include <cmath>

#include "./include/shader.h"
#include "./include//triangle.h"
#include "./include/open_gl_loader.h"
#include "./include/utils.h"

using namespace std;

void waiting_for_movement();

unsigned int VBOs[4], VAOs[4], EBOs[4], cur = 0;

Point center(0.0f, 0.0f, 0.0f);
Point left_point(-0.5f, 0.0f, 0.0f);
Point sinusoidal_point(-0.98f, 0.0f);

std::vector<Triangle> triangles(3);
//Triangle triangle(center, 0.02f);
//Triangle triangle_2(left_point, 0.02f);
//Triangle triangle_3(sinusoidal_point, 0.02f);

int main()
{
    Open_GL_Loader OpenGL(SCR_WIDTH, SCR_HEIGHT);

    Shader current_shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    triangles[0] = Triangle(center, 0.06f);
    triangles[1] = Triangle(left_point, 0.04f);
    triangles[2] = Triangle(sinusoidal_point, 0.02f);

    objectSetterUp<Triangle>(triangles[0], VBOs[0], VAOs[0], EBOs[0]);
    objectSetterUp<Triangle>(triangles[1], VBOs[1], VAOs[1], EBOs[1]);
    objectSetterUp<Triangle>(triangles[2], VBOs[2], VAOs[2], EBOs[2]);

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

        glBindVertexArray(VAOs[2]);
        glDrawElements(mode, _size, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(OpenGL.window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);
    glDeleteBuffers(3, EBOs);
    
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

    triangles[0].move_in_spiral(5.0f, -0.02f);
    triangles[1].move_in_circle(0.5f);
    triangles[2].move_sinusoidal(0.1f, 30.0f);

    update_object(triangles[0], VBOs[0], VAOs[0]);
    update_object(triangles[1], VBOs[1], VAOs[1]);
    update_object(triangles[2], VBOs[2], VAOs[2]);
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
        triangles[0].stop();
        triangles[1].stop();
        triangles[2].stop();
    }

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        cur++;
        cur = cur % 3;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        triangles[cur].move_to_the_left();
        update_object(triangles[cur], VBOs[cur], VAOs[cur]);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        triangles[cur].move_to_the_right();
        update_object(triangles[cur], VBOs[cur], VAOs[cur]);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        triangles[cur].move_up();
        update_object(triangles[cur], VBOs[cur], VAOs[cur]);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        triangles[cur].move_down();
        update_object(triangles[cur], VBOs[cur], VAOs[cur]);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}