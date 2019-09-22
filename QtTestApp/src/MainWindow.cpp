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

auto MainWindow::makeEditMenu(QAction & clearAction)
    -> QMenu &
{
    auto menu = menuBar()->addMenu("Edit");

    menu->addAction(&clearAction);

    return *menu;
}

auto MainWindow::makeToolbar(QAction & clearAction)
    -> QToolBar &
{
    auto bar = new QToolBar{this};

    bar->addAction(&clearAction);

    bar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    addToolBar(Qt::TopToolBarArea, bar);

    return *bar;
}

auto MainWindow::onClear()
    -> void
{
    model->clear();
}
