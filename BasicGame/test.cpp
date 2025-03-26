#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // GLFW������
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    // OpenGL�o�[�W�����w��i��: 3.3 Core Profile�j
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // �E�B���h�E�쐬
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Sample", nullptr, nullptr);
    if (!window) {
        std::cerr << "Window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // GLEW������
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return -1;
    }

    // ���C�����[�v
    while (!glfwWindowShouldClose(window)) {
        // ��ʃN���A�i�w�i�F�j
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window); // �o�b�t�@����ւ�
        glfwPollEvents();        // ���̓C�x���g����
    }

    // �㏈��
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}