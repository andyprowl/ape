#pragma once

#include <QMainWindow>

class QGridLayout;
class TableModel;

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    
    MainWindow(TableModel & model, QWidget *parent = 0);
    
    ~MainWindow();

private:

    auto makeClearAction()
        -> QAction &;

    auto makeEditMenu(QAction & clearAction)
        -> QMenu &;

    auto makeToolbar(QAction & clearAction)
        -> QToolBar &;

    auto onClear()
        -> void;

private:

    TableModel * model;

    QAction * clearAction;

    QMenu * editMenu;

    QToolBar * toolBar;

};
