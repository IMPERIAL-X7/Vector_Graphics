#pragma once
#include "core/Diagram.h"
#include "ui/Canvas.h"
#include <QMainWindow>
#include <QActionGroup>
#include <QLabel>


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();
private:
    void newFile();
    void save();
    void saveAs();
    void openSVG();
    void saveSVG();
    // void copy();
    // void cut();
    // void paste();
    QString currentFile;
    Diagram diagram;
    Command commands;
    Canvas* canvas;
    QActionGroup* toolGroup;
    QAction* selectAction;
    QAction* borderAction;
    QAction* insideAction;
    QAction* thickAction;
    QAction* textAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* circleAction;
    QAction* rectAction;
    QAction* roundRectAction;
    QAction* freehandAction;
    QAction* lineAction;
    QAction* hexagonAction;
    QAction* resizeAction;
    QAction* moveAction;
    QWidget* strokeColorWidget;
    QWidget* fillColorWidget;
    QLabel* strokeLabel;
    QLabel* fillLabel;

    void createMenus();
    void createToolbar();
};