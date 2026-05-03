#include <cmath>
#include <filesystem>
#include <fstream>

#include "./include/shader.h"
#include "./include/house.h"
#include "./include/rectangle.h"
#include "./include/square.h"
#include "./include/open_gl_loader.h"
#include "./include/utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

unsigned int VBOs[4], VAOs[4], EBOs[4];

static void save_ppm(const std::string& path, int w, int h)
{
    std::vector<unsigned char> pixels(w * h * 3);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    std::ofstream f(path, std::ios::binary);
    f << "P6\n" << w << " " << h << "\n255\n";
    for (int row = h - 1; row >= 0; --row)
        f.write(reinterpret_cast<char*>(&pixels[row * w * 3]), w * 3);
}

int main(int argc, char* argv[])
{
    std::string record_dir;
    for (int i = 1; i < argc; ++i)
        if (std::string(argv[i]) == "--record" && i + 1 < argc)
            record_dir = argv[++i];

    const bool recording     = !record_dir.empty();
    const int  record_frames = 120;

    if (recording)
        std::filesystem::create_directories(record_dir);

    Open_GL_Loader OpenGL(SCR_WIDTH, SCR_HEIGHT);
    Shader current_shader(VERTEX_SHADER_TRANSFORM_PATH, FRAGMENT_SHADER_PATH);

    House   house;
    Square  square(Point(0.5f, 0.5f), 0.15f);
    Rectangle rect(Point(-0.5f, -0.5f), 0.1f);

    objectSetterUp<House>(house,     VBOs[0], VAOs[0], EBOs[0]);
    objectSetterUp<Square>(square,   VBOs[1], VAOs[1], EBOs[1]);
    objectSetterUp<Rectangle>(rect,  VBOs[2], VAOs[2], EBOs[2]);

    current_shader.use();
    unsigned int transformLoc = glGetUniformLocation(current_shader.ID, "transform");

    int frame_count = 0;

    while (!glfwWindowShouldClose(OpenGL.window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        current_shader.use();

        float t = recording
            ? (float)frame_count / record_frames * 6.28318f  // full rotation over the clip
            : (float)glfwGetTime();

        // House: spins continuously
        glm::mat4 transform = glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(VAOs[0]);
        glDrawElements(mode, (GLsizei)house.indices.size(), GL_UNSIGNED_INT, 0);

        // Square: scales up and down (pulse)
        float scale = 0.5f + 0.5f * std::sin(t * 2.0f);
        transform = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(VAOs[1]);
        glDrawElements(mode, (GLsizei)square.indices.size(), GL_UNSIGNED_INT, 0);

        // Rectangle: translates left-right (ping-pong)
        float tx = std::sin(t * 1.5f) * 0.4f;
        transform = glm::translate(glm::mat4(1.0f), glm::vec3(tx, 0.0f, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(VAOs[2]);
        glDrawElements(mode, (GLsizei)rect.indices.size(), GL_UNSIGNED_INT, 0);

        if (recording) {
            int fb_w, fb_h;
            glfwGetFramebufferSize(OpenGL.window, &fb_w, &fb_h);
            char path[512];
            snprintf(path, sizeof(path), "%s/frame_%04d.ppm", record_dir.c_str(), frame_count);
            save_ppm(path, fb_w, fb_h);
            if (++frame_count >= record_frames)
                glfwSetWindowShouldClose(OpenGL.window, GLFW_TRUE);
        }

        glfwSwapBuffers(OpenGL.window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);
    glDeleteBuffers(3, EBOs);

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) { glPointSize(8); mode = GL_POINTS; }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) { mode = GL_TRIANGLES; }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}
