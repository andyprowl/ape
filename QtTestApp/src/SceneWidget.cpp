#include "SceneWidget.hpp"

#include <GLFW/glfw3.h>

SceneWidget::SceneWidget(QWidget * const parent)
    : QOpenGLWidget{parent}
{
    // No calls to OpenGL here, do those in initializeGL instead
}

SceneWidget::~SceneWidget()
{
    makeCurrent();

    // Do all resource releasing here

    doneCurrent();
}

// virtual (from QOpenGLWidget)
auto SceneWidget::paintGL()
    -> void
{
    glClearColor(0.3f, 0.5f, 0.5f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// virtual (from QOpenGLWidget)
auto SceneWidget::initializeGL()
    -> void
{
    // Do all initialization here rather than inside the constructo

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
}

// virtual (from QOpenGLWidget)
auto SceneWidget::resizeGL(int const width, int const height)
    -> void
{
    glViewport(0, 0, width, height);
}
