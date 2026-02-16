#include "ui/MainWindow.h"

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

void MainWindow::openSVG() {
  QString file =
      QFileDialog::getOpenFileName(this, "Open SVG", "", "SVG Files (*.svg)");
  if (file.isEmpty()) return;
  currentFile = file;
  CanvasState state = XMLParser::parse(file.toStdString());
  this->diagram = *state.diagram;
  canvas->setFixedSize(state.width, state.height);
  canvas->setDiagram_Cmd(&this->diagram, &commands);
  canvas->update();
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