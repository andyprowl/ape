#include "MainWindow.hpp"
#include "TableModel.hpp"

#include <Rave/RaveCore/RaveEngineFactory.hpp>

#include <Ape/Engine/QtEngine/QtEngine.hpp>
#include <Ape/Engine/QtEngine/QtGateway.hpp>
#include <Ape/Engine/QtEngine/QtWindow.hpp>
#include <Ape/World/Scene/CameraSelector.hpp>

#include <Basix/Range/Search.hpp>

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

auto makeQtWindow(ape::qt::QtGateway & gateway, QWidget & parent)
    -> ape::qt::QtWindow &
{
    auto const widget = gateway.createWindow(parent);

    auto format = QSurfaceFormat{};

    format.setColorSpace(QSurfaceFormat::ColorSpace::sRGBColorSpace);

    format.setVersion(4, 5);

    widget->setFormat(format);

    widget->setTextureFormat(GL_SRGB8_ALPHA8);

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

auto isDebugOutputEnabled(std::vector<std::string> const & arguments)
    -> bool
{
    return basix::contains(arguments, "--enable-debug-output");
}

auto isSponzaExcluded(std::vector<std::string> const & arguments)
    -> bool
{
    return basix::contains(arguments, "--exclude-sponza");
}

int main(int argc, char *argv[])
{
    auto const arguments = std::vector<std::string>{argv, argv + argc};

    auto const enableDebugOutput = isDebugOutputEnabled(arguments);

    auto const excludeSponza = isSponzaExcluded(arguments);

    auto gateway = ape::qt::QtGateway{enableDebugOutput};

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

    auto & sceneView1 = makeQtWindow(gateway, window);

    sceneView1.setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    auto & sceneView2 = makeQtWindow(gateway, window);

    sceneView2.setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    auto & sceneView3 = makeQtWindow(gateway, window);

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

    auto engineFactory = rave::RaveEngineFactory{excludeSponza};

    auto const backgroundColor = glm::vec3{0.0f, 0.0f, 0.0f};
    
    /// ---
    
    auto const engine1 = engineFactory.makeEngine<ape::qt::QtEngine>(sceneView1);

    engine1->getRenderer().getCameraSelector().activateNextCamera();

    engine1->run();

    /// ---

    auto const engine2 = engineFactory.makeEngine<ape::qt::QtEngine>(sceneView2);

    engine2->run();
    
    /// ---

    auto const engine3 = engineFactory.makeEngine<ape::qt::QtEngine>(sceneView3);

    engine3->getRenderer().getCameraSelector().activatePreviousCamera();

    engine3->run();
    
    // ---

    return app.exec();
}
