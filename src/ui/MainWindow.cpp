#include "ui/MainWindow.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenuBar>
#include <QScrollArea>
#include <QStyle>
#include <QToolBar>
#include <QWidgetAction>
#include <iostream>

#include "io/SVGWriter.h"
#include "io/XMLParser.h"

MainWindow::MainWindow() : QMainWindow() {
  QScrollArea* scrollArea = new QScrollArea(this);
  scrollArea->setBackgroundRole(
      QPalette::Dark);                        // Dark background like Inkscape
  scrollArea->setAlignment(Qt::AlignCenter);  // Keeps the canvas in the center
  setCentralWidget(scrollArea);

  canvas = new Canvas(this);
  // commands = new Command;
  scrollArea->setWidget(canvas);
  canvas->setDiagram_Cmd(&diagram, &commands);
  commands.setDiagram_cmd(&diagram);
  createMenus();
  createToolbar();
  canvas->setFixedSize(600, 400);  // Set a fixed size for the canvas
  canvas->setStyleSheet("background-color: white; border: 2px solid #555;");
  canvas->setStyleSheet(
      "QInputDialog { background-color: #404040; color: white; font-size: "
      "14px; }"
      "QDoubleSpinBox { background-color: #202020; color: white; border: 1px "
      "solid gray; }"
      "QLabel { color: white; }");

  resize(800, 600);
}

MainWindow::~MainWindow() {}

void MainWindow::createMenus() {
  QMenu* fileMenu = menuBar()->addMenu("File");
  // Syntax: addAction(Text, Receiver, Member/Slot, Shortcut)
  fileMenu->addAction("New", QKeySequence::New, this, &MainWindow::newFile);
  fileMenu->addAction("Open", QKeySequence::Open, this, &MainWindow::openSVG);
  fileMenu->addAction("Save", QKeySequence::Save, this, &MainWindow::save);
  fileMenu->addAction("Save As", QKeySequence::SaveAs, this,
                      &MainWindow::saveAs);
  fileMenu->addAction("Close", QKeySequence::Close, this, &MainWindow::close);

  QMenu* editMenu = menuBar()->addMenu("Edit");
  editMenu->addAction("Undo", QKeySequence::Undo, this, [this]() {
    commands.undo();
    canvas->update();
  });
  editMenu->addAction("Redo", QKeySequence::Redo, this, [this]() {
    commands.redo();
    canvas->update();
  });
  editMenu->addSeparator();
  editMenu->addAction("Cut", QKeySequence::Cut, this,
                      [this]() { canvas->cut(); });
  editMenu->addAction("Copy", QKeySequence::Copy, this,
                      [this]() { canvas->copy(); });
  editMenu->addAction("Paste", QKeySequence::Paste, this,
                      [this]() { canvas->paste(); });
}

void MainWindow::createToolbar() {
  QToolBar* tools = addToolBar("Tools");
  tools->setOrientation(Qt::Horizontal);
  toolGroup = new QActionGroup(this);
  toolGroup->setExclusive(true);

  // Create actions FIRST
  selectAction = new QAction("Select", this);
  selectAction->setCheckable(true);
  connect(selectAction, &QAction::triggered, this,
          [this]() { canvas->setTool(Tool::Select); });
  toolGroup->addAction(selectAction);

  circleAction = new QAction("Circle", this);
  circleAction->setCheckable(true);
  connect(circleAction, &QAction::triggered, this,
          [this]() { canvas->setTool(Tool::Circle); });
  toolGroup->addAction(circleAction);  // Add AFTER creation

  rectAction = new QAction("Rect", this);
  rectAction->setCheckable(true);
  connect(rectAction, &QAction::triggered, this,
          [this]() { canvas->setTool(Tool::Rect); });
  toolGroup->addAction(rectAction);

  freehandAction = new QAction("Free", this);
  freehandAction->setCheckable(true);
  connect(freehandAction, &QAction::triggered, this,
          [this]() { canvas->setTool(Tool::Freehand); });
  toolGroup->addAction(freehandAction);

  roundRectAction = new QAction("RoundRect", this);
  roundRectAction->setCheckable(true);
  connect(roundRectAction, &QAction::triggered, this,
          [this]() { canvas->setTool(Tool::RoundRect); });
  toolGroup->addAction(roundRectAction);

  hexagonAction = new QAction("Hexagon", this);
  hexagonAction->setCheckable(true);
  connect(hexagonAction, &QAction::triggered, this,
          [this]() { canvas->setTool(Tool::Hexagon); });
  toolGroup->addAction(hexagonAction);

  lineAction = new QAction("Line", this);
  lineAction->setCheckable(true);
  connect(lineAction, &QAction::triggered, this,
          [this]() { canvas->setTool(Tool::Line); });
  toolGroup->addAction(lineAction);

  textAction = new QAction("Text", this);
  textAction->setCheckable(true);
  connect(textAction, &QAction::triggered, this,
          [this]() { canvas->setTool(Tool::Text); });
  toolGroup->addAction(textAction);

  resizeAction = new QAction("Resize", this);
  resizeAction->setCheckable(true);
  connect(resizeAction, &QAction::triggered, this,
          [this]() { canvas->setTool(Tool::Resize); });
  toolGroup->addAction(resizeAction);

  moveAction = new QAction("Move", this);
  moveAction->setCheckable(true);
  connect(moveAction, &QAction::triggered, this,
          [this]() { canvas->setTool(Tool::Move); });
  toolGroup->addAction(moveAction);

  tools->addActions(toolGroup->actions());

  /*
      lambda functions: Basic Syntax:- [capture](parameters) -> return_type {
     body }

      [this]: Gives the lambda access to the current class instance members
     (like canvas).
      [=]: Capture all local variables by value.
      [&]: Capture all local variables by reference.
      []: Capture nothing.
  */

  tools->addSeparator();
  strokeColorWidget = new QWidget();
  strokeColorWidget->setFixedSize(30, 30);
  QPalette strokePal = strokeColorWidget->palette();
  strokePal.setColor(QPalette::Window, Qt::black);  // Default black
  strokeColorWidget->setPalette(strokePal);
  strokeColorWidget->setAutoFillBackground(true);

  borderAction = new QAction("Stroke", this);
  connect(borderAction, &QAction::triggered, this, [this]() {
    QColor color = QColorDialog::getColor(Qt::black, this, "Select Stroke",
                                          QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
      canvas->setcurrStroke(color);
      QPalette palatte = strokeColorWidget->palette();
      palatte.setColor(QPalette::Window, color);
      strokeColorWidget->setPalette(palatte);
    }
  });
  QWidgetAction* strokeAction = new QWidgetAction(this);
  strokeAction->setDefaultWidget(strokeColorWidget);
  tools->addAction(strokeAction);
  tools->addAction(borderAction);

  fillColorWidget = new QWidget();
  fillColorWidget->setFixedSize(30, 30);
  fillColorWidget->setStyleSheet(
      "background-color: white; border: 2px solid gray;");

  insideAction = new QAction("Fill", this);
  connect(insideAction, &QAction::triggered, this, [this]() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Select Fill Color",
                                          QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
      canvas->setcurrFill(color);
      fillColorWidget->setStyleSheet(
          QString("background-color: %1; border: 2px solid gray;")
              .arg(color.name()));
    }
  });
  QWidgetAction* fillAction = new QWidgetAction(this);
  fillAction->setDefaultWidget(fillColorWidget);
  tools->addAction(fillAction);
  tools->addAction(insideAction);

  thickAction = new QAction("strk_width", this);
  connect(thickAction, &QAction::triggered, this, [this]() {
    bool ok;
    canvas->setStrokeWidth(QInputDialog::getDouble(
        this, tr("Stroke Width"), tr("Enter Width:"), 1.0, 0.1, 100.0, 1, &ok));
  });

  tools->addActions({thickAction});
}

void MainWindow::openSVG() {
  QString file =
      QFileDialog::getOpenFileName(this, "Open SVG", "", "SVG Files (*.svg)");
  if (file.isEmpty()) return;
  currentFile = file;
  diagram = XMLParser::parse(file.toStdString());
  canvas->setDiagram_Cmd(&diagram, &commands);
  std::cout << "Opened file: " << file.toStdString() << std::endl;
}

void MainWindow::newFile() {
  diagram.clear();
  currentFile.clear();
  commands = Command();
  canvas->update();
}

void MainWindow::closeFile() {
  MainWindow::save();  // Save before closing
  currentFile.clear();
  diagram.clear();
  commands = Command();
  canvas->update();
}

void MainWindow::save() {
  if (currentFile.isEmpty()) {
    saveAs();
  }
  SVGWriter::write(diagram, currentFile.toStdString());
}

void MainWindow::saveAs() { saveSVG(); }

void MainWindow::saveSVG() {
  QString file_ =
      QFileDialog::getSaveFileName(this, "Save SVG", "", "SVG Files (*.svg)");

  if (file_.isEmpty()) return;
  std::string filePath_ = file_.toStdString() + ".svg";
  SVGWriter::write(diagram, filePath_);
}