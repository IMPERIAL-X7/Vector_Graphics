#include "ui/MainWindow.h"
#include "io/XMLParser.h"
#include "io/SVGWriter.h"
#include <iostream>
#include <QMenuBar>
#include <QToolBar>
#include <QInputDialog>
#include <QFileDialog>
#include <QColorDialog>

MainWindow::MainWindow() : QMainWindow()
{
    canvas = new Canvas(this);
    setCentralWidget(canvas);
    canvas->setDiagram(&diagram);

    createMenus();
    createToolbar();

    resize(800, 800);
}

MainWindow::~MainWindow(){}

void MainWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu("File");

    QAction* openAct = new QAction("Open", this);
    QAction* saveAct = new QAction("Save", this);

    connect(openAct, &QAction::triggered, this, &MainWindow::openSVG);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveSVG);

    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
}

void MainWindow::createToolbar()
{
    QToolBar* tools = addToolBar("Tools");
    tools->setOrientation(Qt::Horizontal);

    circleAction = new QAction("Circle", this);
    circleAction->setCheckable(true);
    connect(circleAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Circle);});
    /*
        lambda functions: Basic Syntax:- [capture](parameters) -> return_type { body }

        [this]: Gives the lambda access to the current class instance members (like canvas).
        [=]: Capture all local variables by value.
        [&]: Capture all local variables by reference.
        []: Capture nothing.
    */
    rectAction = new QAction("Rect", this);
    rectAction->setCheckable(true);
    connect(rectAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Rect);});

    resizeAction = new QAction("Resize", this);
    resizeAction->setCheckable(true);
    connect(resizeAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Resize);});

    moveAction = new QAction("Move", this);
    moveAction->setCheckable(true);
    connect(moveAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Move);});

    textAction = new QAction("Text", this);
    textAction->setCheckable(true);
    connect(textAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Text);});

    roundRectAction = new QAction("RoundRect", this);
    roundRectAction->setCheckable(true);
    connect(roundRectAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::RoundRect);});

    hexagonAction = new QAction("Hexagon", this);
    hexagonAction->setCheckable(true);
    connect(hexagonAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Hexagon);});

    lineAction = new QAction("Line", this);
    lineAction->setCheckable(true);
    connect(lineAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Line);});

    borderAction = new QAction("Border_C", this);
    borderAction->setCheckable(true);
    connect(borderAction, &QAction::triggered, this, [this](){canvas->setcurrStroke(QColorDialog::getColor(Qt::white, this, "Select Color"));});

    insideAction = new QAction("Fill_C", this);
    insideAction->setCheckable(true);
    connect(insideAction, &QAction::triggered, this, [this](){if(!insideAction->isChecked()) canvas->setcurrFill(QColorDialog::getColor(Qt::white, this, "Select Color")); else canvas->setcurrFill(Qt::NoBrush);});

    thickAction = new QAction("strk_width", this);
    thickAction->setCheckable(true);
    connect(thickAction, &QAction::triggered, this, [this](){bool ok; canvas->setStrokeWidth(QInputDialog::getDouble(this, tr("Round Rect"), tr("Enter Corner Radius:"), 10, 0, 10000, 1, &ok));});

    undoAction = new QAction("Undo", this);
    undoAction->setCheckable(true);
    connect(undoAction, &QAction::triggered, this, [this](){diagram.undo(); canvas->update();});

    redoAction = new QAction("Redo", this);
    redoAction->setCheckable(true);
    connect(redoAction, &QAction::triggered, this, [this](){diagram.redo();canvas->update();});

    tools->addActions({circleAction, rectAction, roundRectAction, hexagonAction, lineAction, borderAction, insideAction, thickAction, textAction, undoAction, redoAction, resizeAction, moveAction});

}

void MainWindow::openSVG()
{
    QString file = QFileDialog::getOpenFileName(
        this, "Open XML", "", "XML Files (*.svg *.xml)"
    );
    if(file.isEmpty()) return;

    diagram = XMLParser::parse(file.toStdString());
    canvas->setDiagram(&diagram);
}

void MainWindow::saveSVG() {

    QString file_ = QFileDialog::getSaveFileName(
        this, "Save SVG", "", "XML Files (*.svg, *.xml)"
    );

    if (file_.isEmpty()) return;
    // std::cout << file.toStdString() << std::endl;
    std::string filePath_ = file_.toStdString() + ".xml";
    SVGWriter::write(diagram, filePath_);
}