#pragma once
#include <QActionGroup>
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QScrollArea>
#include <QStyle>
#include <QToolBar>
#include <QWidgetAction>

#include "core/Diagram.h"
#include "ui/Canvas.h"

class MainWindow : public QMainWindow {
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
  void closeFile();

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