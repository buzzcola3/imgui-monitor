#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <thread>

namespace {
constexpr int kWidth = 640;
constexpr int kHeight = 480;
constexpr int kMsPerFrame = 16;
constexpr std::chrono::seconds kRunTime{2};
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(kWidth, kHeight, "GLFW Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    const auto deadline = std::chrono::steady_clock::now() + kRunTime;
    while (!glfwWindowShouldClose(window) && std::chrono::steady_clock::now() < deadline) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(kMsPerFrame));
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
