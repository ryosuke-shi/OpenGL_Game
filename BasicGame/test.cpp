#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // GLFW初期化
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    // OpenGLバージョン指定（例: 3.3 Core Profile）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // ウィンドウ作成
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Sample", nullptr, nullptr);
    if (!window) {
        std::cerr << "Window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // GLEW初期化
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return -1;
    }

    // メインループ
    while (!glfwWindowShouldClose(window)) {
        // 画面クリア（背景色）
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window); // バッファ入れ替え
        glfwPollEvents();        // 入力イベント処理
    }

    // 後処理
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}