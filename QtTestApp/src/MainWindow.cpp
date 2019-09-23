#include "MainWindow.hpp"

#include "TableModel.hpp"

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
    , editMenu{&makeEditMenu(*clearAction)}
    , toolBar{&makeToolbar(*clearAction)}
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

auto MainWindow::makeEditMenu(QAction & action)
    -> QMenu &
{
    auto menu = menuBar()->addMenu("Edit");

    menu->addAction(&action);

    return *menu;
}

auto MainWindow::makeToolbar(QAction & action)
    -> QToolBar &
{
    auto bar = new QToolBar{this};

    bar->addAction(&action);

    bar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    addToolBar(Qt::TopToolBarArea, bar);

    return *bar;
}

auto MainWindow::onClear()
    -> void
{
    model->clear();
}
