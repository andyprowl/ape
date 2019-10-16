#include "MainWindow.hpp"
#include "TableModel.hpp"

#include <Application/TestScene/SampleAssetBuilder.hpp>
#include <Application/TestScene/SampleInputHandler.hpp>
#include <Application/TestScene/SampleSceneBuilder.hpp>

#include <Ape/QtEngine/QtEngine.hpp>
#include <Ape/QtEngine/QtWindow.hpp>
#include <Ape/Rendering/DepthShaderProgram.hpp>
#include <Ape/Rendering/LineStyleProvider.hpp>
#include <Ape/Rendering/SceneRenderer.hpp>
#include <Ape/Rendering/ShapeArrayObjectRenderer.hpp>
#include <Ape/Rendering/ShapeBufferObjectRenderer.hpp>
#include <Ape/Rendering/StandardShaderProgram.hpp>
#include <Ape/Rendering/WireframeShaderProgram.hpp>
#include <Ape/Scene/BodySelector.hpp>
#include <Ape/Scene/CameraSelector.hpp>

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

    format.setVersion(4, 5);

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

    window.registerQtWindow(sceneView1);

    window.registerQtWindow(sceneView2);

    window.registerQtWindow(sceneView3);

    window.show();

    auto const sharing = QOpenGLContext::areSharing(sceneView1.context(), sceneView2.context());

    std::cout << "Sharing is " << (sharing ? "enabled" : "disabled") << std::endl;

    sceneView1.makeCurrent();

    auto assets = createSampleAssets();
    
    auto scene = createSampleScene(assets);

    auto standardShader = ape::StandardShaderProgram{};

    auto depthShader = ape::DepthShaderProgram{};

    auto wireframeShader = ape::WireframeShaderProgram{};

    auto picker = ape::BodySelector{scene};

    auto wireframeStyleProvider = ape::LineStyleProvider{{0.05f, {0.2f, 0.2f, 1.0f}}};

    auto const backgroundColor = glm::vec3{0.0f, 0.0f, 0.0f};
    
    /// ---

    auto selector1 = ape::CameraSelector{scene};

    // Important: fallback VAO in renderer as well as VAOs in ShapeRenderer must be created in the
    // corresponding rendering context!
    sceneView1.makeCurrent();
    
    auto shapeRenderer1 = std::make_unique<ape::ShapeArrayObjectRenderer>(assets.shapes);
    //auto shapeRenderer1 = std::make_unique<ape::ShapeBufferObjectRenderer>();

    auto depthBodyRenderer1 = ape::DepthBodyRenderer{depthShader, *shapeRenderer1};

    auto standardBodyRenderer1 = ape::StandardBodyRenderer{standardShader, *shapeRenderer1};

    auto wireframeBodyRenderer1 = ape::WireframeBodyRenderer{
        wireframeShader,
        *shapeRenderer1,
        wireframeStyleProvider};

    auto outlinedBodyRenderer1 = ape::OutlinedBodyRenderer{
        standardBodyRenderer1,
        wireframeBodyRenderer1};

    auto renderer1 = ape::SceneRenderer{
        std::move(shapeRenderer1),
        std::move(depthBodyRenderer1),
        std::move(standardBodyRenderer1),
        std::move(wireframeBodyRenderer1),
        std::move(outlinedBodyRenderer1),
        selector1,
        picker,
        sceneView1,
        ape::Viewport{{0, 0}, sceneView1.getSize()},
        backgroundColor};

    auto inputHandler1 = SampleInputHandler{
        sceneView1,
        selector1,
        picker,
        standardShader,
        wireframeStyleProvider,
        scene};

    auto engine1 = ape::qt::QtEngine{sceneView1, renderer1, inputHandler1};

    engine1.start();
    
    /// ---

    auto selector2 = ape::CameraSelector{scene};

    // Important: fallback VAO in renderer as well as VAOs in ShapeRenderer must be created in the
    // corresponding rendering context!
    sceneView2.makeCurrent();

    auto shapeRenderer2 = std::make_unique<ape::ShapeArrayObjectRenderer>(assets.shapes);

    auto depthBodyRenderer2 = ape::DepthBodyRenderer{depthShader, *shapeRenderer2};

    auto standardBodyRenderer2 = ape::StandardBodyRenderer{standardShader, *shapeRenderer2};

    auto wireframeBodyRenderer2 = ape::WireframeBodyRenderer{
        wireframeShader,
        *shapeRenderer2,
        wireframeStyleProvider};

    auto outlinedBodyRenderer2 = ape::OutlinedBodyRenderer{
        standardBodyRenderer2,
        wireframeBodyRenderer2};

    auto renderer2 = ape::SceneRenderer{
        std::move(shapeRenderer2),
        std::move(depthBodyRenderer2),
        std::move(standardBodyRenderer2),
        std::move(wireframeBodyRenderer2),
        std::move(outlinedBodyRenderer2),
        selector2,
        picker,
        sceneView2,
        ape::Viewport{{0, 0}, sceneView2.getSize()},
        backgroundColor};

    auto inputHandler2 = SampleInputHandler{
        sceneView2,
        selector2,
        picker,
        standardShader,
        wireframeStyleProvider,
        scene};

    auto engine2 = ape::qt::QtEngine{sceneView2, renderer2, inputHandler2};

    selector2.activateNextCamera();

    engine2.start();
    
    /// ---
    
    auto selector3 = ape::CameraSelector{scene};

    // Important: fallback VAO in renderer as well as VAOs in ShapeRenderer must be created in the
    // corresponding rendering context!
    sceneView3.makeCurrent();
     
    auto shapeRenderer3 = std::make_unique<ape::ShapeArrayObjectRenderer>(assets.shapes);

    auto depthBodyRenderer3 = ape::DepthBodyRenderer{depthShader, *shapeRenderer3};

    auto standardBodyRenderer3 = ape::StandardBodyRenderer{standardShader, *shapeRenderer3};

    auto wireframeBodyRenderer3 = ape::WireframeBodyRenderer{
        wireframeShader,
        *shapeRenderer3,
        wireframeStyleProvider};

    auto outlinedBodyRenderer3 = ape::OutlinedBodyRenderer{
        standardBodyRenderer3,
        wireframeBodyRenderer3};

    auto renderer3 = ape::SceneRenderer{
        std::move(shapeRenderer3),
        std::move(depthBodyRenderer3),
        std::move(standardBodyRenderer3),
        std::move(wireframeBodyRenderer3),
        std::move(outlinedBodyRenderer3),
        selector3,
        picker,
        sceneView3,
        ape::Viewport{{0, 0}, sceneView3.getSize()},
        backgroundColor};

    auto inputHandler3 = SampleInputHandler{
        sceneView3,
        selector3,
        picker,
        standardShader,
        wireframeStyleProvider,
        scene};

    auto engine3 = ape::qt::QtEngine{sceneView3, renderer3, inputHandler3};

    selector3.activatePreviousCamera();

    engine3.start();
    
    // ---

    return app.exec();
}
