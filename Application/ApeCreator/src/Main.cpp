#include "MainWindow.hpp"
#include "TableModel.hpp"

#include <TestScene/SampleAssetBuilder.hpp>
#include <TestScene/SampleInputHandler.hpp>
#include <TestScene/SampleSceneBuilder.hpp>

#include <QtEngine/QtEngine.hpp>
#include <QtEngine/QtWindow.hpp>

#include <UpdateHandling/OpenGLLoader.hpp>

#include <Rendering/SceneRenderer.hpp>
#include <Rendering/ShapeArrayObjectRenderer.hpp>
#include <Rendering/StandardShaderProgram.hpp>
#include <Rendering/WireframeShaderProgram.hpp>

#include <Scene/BodySelector.hpp>
#include <Scene/CameraSelector.hpp>

#include <QApplication>
#include <QFileSystemModel>
#include <QGridLayout>
#include <QListView>
#include <QScreen>
#include <QTableView>
#include <QTreeView>
#include <QWidget>

#include <iostream>

template<typename Model>
auto makeListView(QWidget & parent, Model & model)
    -> QListView &
{
    auto view = new QListView{&parent};

    view->setModel(&model);

    return *view;
}

template<typename Model>
auto makeTableView(QWidget & parent, Model & model)
    -> QTableView &
{
    auto view = new QTableView{&parent};

    view->setModel(&model);

    return *view;
}

template<typename Model>
auto makeTreeView(QWidget & parent, Model & model)
    -> QTreeView &
{
    auto view = new QTreeView{&parent};

    view->setModel(&model);

    return *view;
}

auto makeQtWindow(QWidget & parent)
    -> ape::qt::QtWindow &
{
    auto const widget = new ape::qt::QtWindow{&parent};

    auto format = QSurfaceFormat{};

    format.setVersion(4, 3);

    widget->setFormat(format);

    return *widget;
}

auto makeCentralWidget()
    -> QWidget &
{
    auto widget = new QWidget{};

    auto layout = new QGridLayout{widget};

    widget->setLayout(layout);

    return *widget;
}

auto getGridLayout(QWidget & widget)
    -> QGridLayout &
{
    return static_cast<QGridLayout &>(*widget.layout());
}

int main(int argc, char *argv[])
{
    auto && app = QApplication{argc, argv};
    
    auto && model = TableModel{2, 3};

    auto && window = MainWindow{model};

    auto && fsModel = QFileSystemModel{};

    fsModel.setRootPath(QDir::currentPath());

    auto & centralWidget = makeCentralWidget();

    window.setCentralWidget(&centralWidget);

    auto & tableView1 = makeTableView(centralWidget, model);

    auto & treeView1 = makeTreeView(centralWidget, model);

    auto & treeView2 = makeTreeView(centralWidget, fsModel);

    treeView2.setRootIndex(fsModel.index(QString{"c:\\temp"}));

    auto & listView1 = makeListView(centralWidget, model);

    auto & sceneView1 = makeQtWindow(window);

    sceneView1.setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    auto & sceneView2 = makeQtWindow(window);

    sceneView2.setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    auto & sceneView3 = makeQtWindow(window);

    sceneView3.setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    getGridLayout(centralWidget).addWidget(&tableView1, 0, 0);

    getGridLayout(centralWidget).addWidget(&sceneView1, 0, 1);

    getGridLayout(centralWidget).addWidget(&treeView1, 0, 2);

    getGridLayout(centralWidget).addWidget(&sceneView2, 1, 0);

    getGridLayout(centralWidget).addWidget(&listView1, 1, 1);

    getGridLayout(centralWidget).addWidget(&sceneView3, 1, 2);
    
    getGridLayout(centralWidget).setColumnStretch(0, 1.0);
    
    getGridLayout(centralWidget).setColumnStretch(1, 1.0);
    
    getGridLayout(centralWidget).setColumnStretch(2, 1.0);

    treeView1.setSelectionModel(tableView1.selectionModel());

    treeView2.setSelectionModel(tableView1.selectionModel());

    listView1.setSelectionModel(tableView1.selectionModel());

    auto const screenSize = QApplication::screens().front()->size();

    window.resize(screenSize.width(), screenSize.height());

    window.show();

    window.registerQtWindow(sceneView1);

    window.registerQtWindow(sceneView2);

    window.registerQtWindow(sceneView3);

    // The OpenGL context has been created after window.show() was called
    auto const loader = ape::OpenGLLoader{true, true};

    auto const sharing = QOpenGLContext::areSharing(sceneView1.context(), sceneView2.context());

    std::cout << "Sharing is " << (sharing ? "enabled" : "disabled") << std::endl;

    sceneView1.makeCurrent();

    auto assets = createSampleAssets();
    
    auto scene = createSampleScene(assets);

    auto standardShader = ape::StandardShaderProgram{};

    auto wireframeShader = ape::WireframeShaderProgram{};

    auto picker = ape::BodySelector{scene};

    auto const backgroundColor = glm::vec3{0.0f, 0.0f, 0.0f};
    
    /// ---

    auto selector1 = ape::CameraSelector{scene};

    // Important: fallback VAO in renderer as well as VAOs in ShapeRenderer must be created in the
    // corresponding rendering context!
    sceneView1.makeCurrent();

    auto const shapeRenderer1 = ape::ShapeArrayObjectRenderer{assets.shapes};

    auto renderer1 = ape::SceneRenderer{
        selector1,
        picker,
        shapeRenderer1,
        standardShader,
        wireframeShader,
        backgroundColor};

    auto inputHandler1 = SampleInputHandler{
        sceneView1,
        selector1,
        picker,
        standardShader,
        renderer1,
        scene};

    auto engine1 = ape::qt::QtEngine{sceneView1, renderer1, inputHandler1};

    sceneView1.engage(renderer1);

    engine1.start();

    /// ---

    auto selector2 = ape::CameraSelector{scene};

    // Important: fallback VAO in renderer as well as VAOs in ShapeRenderer must be created in the
    // corresponding rendering context!
    sceneView2.makeCurrent();

    auto const shapeRenderer2 = ape::ShapeArrayObjectRenderer{assets.shapes};

    auto renderer2 = ape::SceneRenderer{
        selector2,
        picker,
        shapeRenderer2,
        standardShader,
        wireframeShader,
        backgroundColor};

    auto inputHandler2 = SampleInputHandler{
        sceneView2,
        selector2,
        picker,
        standardShader,
        renderer2,
        scene};

    auto engine2 = ape::qt::QtEngine{sceneView2, renderer2, inputHandler2};

    selector2.activateNextCamera();

    sceneView2.engage(renderer2);

    engine2.start();

    /// ---

    auto selector3 = ape::CameraSelector{scene};

    // Important: fallback VAO in renderer as well as VAOs in ShapeRenderer must be created in the
    // corresponding rendering context!
    sceneView3.makeCurrent();

    auto const shapeRenderer3 = ape::ShapeArrayObjectRenderer{assets.shapes};

    auto renderer3 = ape::SceneRenderer{
        selector3,
        picker,
        shapeRenderer3,
        standardShader,
        wireframeShader,
        backgroundColor};

    auto inputHandler3 = SampleInputHandler{
        sceneView3,
        selector3,
        picker,
        standardShader,
        renderer3,
        scene};

    auto engine3 = ape::qt::QtEngine{sceneView3, renderer3, inputHandler3};

    selector3.activatePreviousCamera();

    sceneView3.engage(renderer3);

    engine3.start();

    // ---

    return app.exec();
}
