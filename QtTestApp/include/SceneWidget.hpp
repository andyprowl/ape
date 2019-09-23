#pragma once

#include <Ape/Engine.hpp>
#include <Ape/GLFWGateway.hpp>
#include <Ape/SceneRenderer.hpp>
#include <Ape/StandardInputHandler.hpp>
#include <Ape/StandardShaderProgram.hpp>

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

class SceneWidget : public QOpenGLWidget, public QOpenGLFunctions
{

    Q_OBJECT;

public:

    explicit SceneWidget(QWidget * parent);

    ~SceneWidget();

private:

    // virtual (from QOpenGLWidget)
    auto paintGL()
        -> void override;

    // virtual (from QOpenGLWidget)
    auto initializeGL()
        -> void override;

    // virtual (from QOpenGLWidget)
    auto resizeGL(int width, int height)
        -> void override;

private:

    class EngineData;

private:

    std::shared_ptr<EngineData> data;

};