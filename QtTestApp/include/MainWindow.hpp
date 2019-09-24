#pragma once

#include <QMainWindow>

class QGridLayout;
class SceneWidget;
class TableModel;

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    
    MainWindow(TableModel & model, QWidget *parent = 0);
    
    ~MainWindow();

    auto registerSceneWidget(SceneWidget & widget)
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

    auto onFocusSceneWidget1()
        -> void;

    auto onFocusSceneWidget2()
        -> void;

    auto onNoFocusSceneWidget()
        -> void;

private:

    TableModel * model;

    QAction * clearAction;

    std::vector<QAction *> focusActions;

    QMenu * editMenu;

    QMenu * viewMenu;

    QToolBar * toolBar;

    std::vector<SceneWidget *> sceneWidgets;

};
