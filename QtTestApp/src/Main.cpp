#include "MainWindow.hpp"
#include "TableModel.hpp"

#include <TestScene/SampleAssetBuilder.hpp>
#include <TestScene/SampleInputHandler.hpp>
#include <TestScene/SampleSceneBuilder.hpp>

#include <QtWindow/SceneWidget.hpp>

#include <Core/CameraSelector.hpp>
#include <Core/OpenGLLoader.hpp>
#include <Core/RenderingContext.hpp>
#include <Core/StandardShaderProgram.hpp>

#include <QApplication>
#include <QFileSystemModel>
#include <QGridLayout>
#include <QListView>
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

auto makeSceneWidget(QWidget & parent)
    -> ape::qt::SceneWidget &
{
    auto const context = ape::RenderingContext{ape::RenderingPolicy::useArrayObjects};

    auto const widget = new ape::qt::SceneWidget{context, &parent};

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

    auto & sceneView1 = makeSceneWidget(window);

    sceneView1.setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    auto & sceneView2 = makeSceneWidget(window);

    sceneView2.setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    getGridLayout(centralWidget).addWidget(&tableView1, 0, 0);

    getGridLayout(centralWidget).addWidget(&treeView1, 0, 1);

    getGridLayout(centralWidget).addWidget(&treeView2, 1, 0);

    getGridLayout(centralWidget).addWidget(&listView1, 1, 1);

    getGridLayout(centralWidget).addWidget(&sceneView1, 0, 2);

    getGridLayout(centralWidget).addWidget(&sceneView2, 1, 2);
    
    getGridLayout(centralWidget).setColumnStretch(0, 1.0);
    
    getGridLayout(centralWidget).setColumnStretch(1, 1.0);
    
    getGridLayout(centralWidget).setColumnStretch(2, 1.0);

    treeView1.setSelectionModel(tableView1.selectionModel());

    treeView2.setSelectionModel(tableView1.selectionModel());

    listView1.setSelectionModel(tableView1.selectionModel());

    window.resize(1024, 768);

    window.show();

    window.registerSceneWidget(sceneView1);

    window.registerSceneWidget(sceneView2);

    // The OpenGL context has been created after window.show() was called
    ape::OpenGLLoader loader{true, true};

    auto const sharing = QOpenGLContext::areSharing(sceneView1.context(), sceneView2.context());

    std::cout << "Sharing is " << (sharing ? "enabled" : "disabled") << std::endl;

    sceneView1.makeCurrent();

    auto assets = createSampleAssets();
    
    auto scene = createSampleScene(assets);

    auto selector1 = ape::CameraSelector{scene};

    auto shader = ape::StandardShaderProgram{};

    auto inputHandler1 = SampleInputHandler{sceneView1, selector1, shader, scene};

    sceneView1.engage(selector1, inputHandler1, shader);

    auto selector2 = ape::CameraSelector{scene};

    auto inputHandler2 = SampleInputHandler{sceneView2, selector2, shader, scene};

    sceneView2.engage(selector2, inputHandler2, shader);

    sceneView2.getCameraSelector().activateNextCamera();

    return app.exec();
}
