//TRANSFORMATIONS CHAPTER

#include <cmath>

#include "./include/shader.h"
#include "./include/triangle.h"
#include "./include/rectangle.h"
#include "./include/house.h"
#include "./include/open_gl_loader.h"
#include "./include/utils.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void waiting_for_movement();

unsigned int VBOs[4], VAOs[4], EBOs[4], cur = 0;

Point center(0.0f, 0.0f, 0.0f);
Point left_point(-0.5f, 0.0f, 0.0f);
Point sinusoidal_point(-0.98f, 0.0f);

House render_object;

int main()
{
    Open_GL_Loader OpenGL(SCR_WIDTH, SCR_HEIGHT);
    Shader current_shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    objectSetterUp<House>(render_object, VBOs[0], VAOs[0], EBOs[0]);

    ///

    //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    //glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    //vec = trans * vec;
    //std::cout << vec.x << vec.y << vec.z << std::endl;

    //glm::mat4 trans = glm::mat4(1.0f); //Identity matrix
    //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); //Then Rotate 90 degrees to the left
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); // First scale by its mitad
    
    //unsigned int transformLoc = glGetUniformLocation(current_shader.ID, "transform");
    //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    ///

    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    //transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    current_shader.use();

    unsigned int transformLoc = glGetUniformLocation(current_shader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    

    while (!glfwWindowShouldClose(OpenGL.window))
    {
        waiting_for_movement();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        current_shader.use(); //Using previously compiled shaders

        //unsigned int transformLoc = glGetUniformLocation(current_shader.ID, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VAOs[0]);
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
void update_object(ModifiedObject& currentObject, unsigned int _VBO, unsigned int _VAO)
{
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, currentObject.size() * sizeof(currentObject.get_vertices()), &currentObject.front(), GL_STATIC_DRAW);
}

void waiting_for_movement()
{
    if (!start) return;

    render_object.move_in_spiral(5.0f, -0.02f);

    update_object<House>(render_object, VBOs[0], VAOs[0]);
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
        render_object.stop();
    }

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        cur++;
        cur = cur % 1;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        render_object.move_to_the_left();
        update_object(render_object, VBOs[cur], VAOs[cur]);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        render_object.move_to_the_right();
        update_object(render_object, VBOs[cur], VAOs[cur]);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        render_object.move_up();
        update_object(render_object, VBOs[cur], VAOs[cur]);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        render_object.move_down();
        update_object(render_object, VBOs[cur], VAOs[cur]);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}