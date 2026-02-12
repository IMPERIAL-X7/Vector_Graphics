#pragma once
#include "core/Diagram.h"
#include "ui/Canvas.h"
#include <QMainWindow>


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();
private:
    void openSVG();
    void saveSVG();

    Diagram diagram;
    Canvas* canvas;
    QAction* circleAction;
    QAction* rectAction;
    QAction* roundRectAction;
    QAction* hexagonAction;
    QAction* lineAction;
    QAction* borderAction;
    QAction* insideAction;
    QAction* thickAction;
    QAction* textAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* resizeAction;
    QAction* moveAction;
    
    void createMenus();
    void createToolbar();
};