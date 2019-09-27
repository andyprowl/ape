#include "MainWindow.hpp"

#include "TableModel.hpp"

#include <QtWindow/SceneWidget.hpp>

#include <QAbstractItemView>
#include <QAction>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

MainWindow::MainWindow(TableModel & model, QWidget *parent)
    : QMainWindow(parent)
    , model{&model}
    , clearAction{&makeClearAction()}
    , focusActions{makeFocusActions()}
    , editMenu{&makeEditMenu()}
    , viewMenu{&makeViewMenu()}
    , toolBar{&makeToolbar()}
{
    statusBar()->showMessage("Ready.");
}

MainWindow::~MainWindow()
{
}

auto MainWindow::makeClearAction()
    -> QAction &
{
    auto action = new QAction{"Clear", this};

    action->setToolTip("Clear the view contents");

    action->setShortcut(QKeySequence::Delete);

    action->setStatusTip("Deletes all the items in the shared model");

    connect(action, &QAction::triggered, this, &MainWindow::onClear);

    return *action;
}

auto MainWindow::makeFocusActions()
    -> std::vector<QAction *>
{
    auto actions = std::vector<QAction *>{};

    auto action = actions.emplace_back(new QAction{"Scene Widget 1", this});

    action->setToolTip("Focus Scene Widge 1");

    connect(action, &QAction::triggered, this, &MainWindow::onFocusSceneWidget1);

    action = actions.emplace_back(new QAction{"Scene Widget 2", this});

    action->setToolTip("Focus Scene Widge 2");

    connect(action, &QAction::triggered, this, &MainWindow::onFocusSceneWidget2);

    action = actions.emplace_back(new QAction{"No Scene Widget", this});

    action->setToolTip("No Focus Scene Widget");

    connect(action, &QAction::triggered, this, &MainWindow::onNoFocusSceneWidget);

    return actions;
}

auto MainWindow::makeEditMenu()
    -> QMenu &
{
    auto menu = menuBar()->addMenu("Edit");

    menu->addAction(clearAction);

    return *menu;
}

auto MainWindow::makeViewMenu()
    -> QMenu &
{
    auto menu = menuBar()->addMenu("View");

    menu->addAction(focusActions[0]);

    menu->addAction(focusActions[1]);

    menu->addAction(focusActions[2]);

    return *menu;
}

auto MainWindow::makeToolbar()
    -> QToolBar &
{
    auto bar = new QToolBar{this};

    bar->addAction(clearAction);

    bar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    addToolBar(Qt::TopToolBarArea, bar);

    return *bar;
}

auto MainWindow::onClear()
    -> void
{
    model->clear();
}

auto MainWindow::onFocusSceneWidget1()
    -> void
{
    sceneWidgets[0]->setFocus();
}

auto MainWindow::onFocusSceneWidget2()
    -> void
{
    sceneWidgets[1]->setFocus();
}

auto MainWindow::onNoFocusSceneWidget()
    -> void
{
    setFocus(Qt::FocusReason::OtherFocusReason);
}

auto MainWindow::registerSceneWidget(ape::qt::SceneWidget & widget)
    -> void
{
    sceneWidgets.push_back(&widget);
}
