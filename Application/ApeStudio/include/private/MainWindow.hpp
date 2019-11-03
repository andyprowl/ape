#pragma once

#include <QMainWindow>

namespace ape::qt
{

class QtWindow;

} // namespace ape::qt

class QGridLayout;
class TableModel;

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    
    MainWindow(TableModel & model, QWidget *parent = 0);
    
    ~MainWindow();

    auto registerQtWindow(ape::qt::QtWindow & widget)
        -> void;

private:

    auto makeClearAction()
        -> QAction &;

    auto makeFocusActions()
        -> std::vector<QAction *>;

    auto makeEditMenu()
        -> QMenu &;

    auto makeViewMenu()
        -> QMenu &;

    auto makeToolbar()
        -> QToolBar &;

    auto onClear()
        -> void;

    auto onFocusQtWindow1()
        -> void;

    auto onFocusQtWindow2()
        -> void;

    auto onNoFocusQtWindow()
        -> void;

private:

    TableModel * model;

    QAction * clearAction;

    std::vector<QAction *> focusActions;

    QMenu * editMenu;

    QMenu * viewMenu;

    QToolBar * toolBar;

    std::vector<ape::qt::QtWindow *> QtWindows;

};
