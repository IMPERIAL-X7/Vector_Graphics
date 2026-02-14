#include "ui/MainWindow.h"
#include "io/XMLParser.h"
#include "io/SVGWriter.h"
#include <iostream>
#include <QMenuBar>
#include <QToolBar>
#include <QInputDialog>
#include <QFileDialog>
#include <QColorDialog>
#include <QWidgetAction>

MainWindow::MainWindow() : QMainWindow()
{
    canvas = new Canvas(this);
    // commands = new Command;
    setCentralWidget(canvas);
    canvas->setDiagram_Cmd(&diagram, &commands);
    commands.setDiagram_cmd(&diagram);
    createMenus();
    createToolbar();

    resize(800, 800);
}

MainWindow::~MainWindow(){}


void MainWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu("File");
    QMenu* copyMenu = menuBar()->addMenu("Copy");

    QAction* openAct = new QAction("Open", this);
    QAction* saveAct = new QAction("Save", this);
    QAction* copyAct = new QAction("Copy", this);
    QAction* cutAct = new QAction("Cut", this);
    QAction* pasteAct = new QAction("Paste", this);

    connect(openAct, &QAction::triggered, this, &MainWindow::openSVG);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveSVG);
    connect(copyAct, &QAction::triggered, this, [this](){canvas->copy();});
    connect(cutAct, &QAction::triggered, this, [this](){canvas->cut();});
    connect(pasteAct, &QAction::triggered, this, [this](){canvas->paste();});

    copyMenu->addAction(copyAct);
    copyMenu->addAction(cutAct);
    copyMenu->addAction(pasteAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
}

void MainWindow::createToolbar()
{
    QToolBar* tools = addToolBar("Tools");
    tools->setOrientation(Qt::Horizontal);
    toolGroup = new QActionGroup(this);
    toolGroup->setExclusive(true);


    // Create actions FIRST
    selectAction = new QAction("Select", this);
    selectAction->setCheckable(true);
    connect(selectAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Select);});
    toolGroup->addAction(selectAction);

    circleAction = new QAction("Circle", this);
    circleAction->setCheckable(true);
    connect(circleAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Circle);});
    toolGroup->addAction(circleAction); // Add AFTER creation

    rectAction = new QAction("Rect", this);
    rectAction->setCheckable(true);
    connect(rectAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Rect);});
    toolGroup->addAction(rectAction);

    roundRectAction = new QAction("RoundRect", this);
    roundRectAction->setCheckable(true);
    connect(roundRectAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::RoundRect);});
    toolGroup->addAction(roundRectAction);

    hexagonAction = new QAction("Hexagon", this);
    hexagonAction->setCheckable(true);
    connect(hexagonAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Hexagon);});
    toolGroup->addAction(hexagonAction);

    lineAction = new QAction("Line", this);
    lineAction->setCheckable(true);
    connect(lineAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Line);});
    toolGroup->addAction(lineAction);

    textAction = new QAction("Text", this);
    textAction->setCheckable(true);
    connect(textAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Text);});
    toolGroup->addAction(textAction);

    resizeAction = new QAction("Resize", this);
    resizeAction->setCheckable(true);
    connect(resizeAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Resize);});
    toolGroup->addAction(resizeAction);

    moveAction = new QAction("Move", this);
    moveAction->setCheckable(true);
    connect(moveAction, &QAction::triggered, this, [this](){canvas->setTool(Tool::Move);});
    toolGroup->addAction(moveAction);

    tools->addActions(toolGroup->actions());

    /*
        lambda functions: Basic Syntax:- [capture](parameters) -> return_type { body }

        [this]: Gives the lambda access to the current class instance members (like canvas).
        [=]: Capture all local variables by value.
        [&]: Capture all local variables by reference.
        []: Capture nothing.
    */

    // borderAction = new QAction("Border_C", this);
    // borderAction->setCheckable(true);
    // connect(borderAction, &QAction::triggered, this, [this](){canvas->setcurrStroke(QColorDialog::getColor(Qt::white, this, "Select Color", QColorDialog::ShowAlphaChannel));});

    // insideAction = new QAction("Fill_C", this);
    // insideAction->setCheckable(true);
    // connect(insideAction, &QAction::triggered, this, [this](){if(insideAction->isChecked()) canvas->setcurrFill(QColorDialog::getColor(Qt::white, this, "Select Color", QColorDialog::ShowAlphaChannel)); else canvas->setcurrFill(Qt::NoBrush);});

    // thickAction = new QAction("strk_width", this);
    // thickAction->setCheckable(true);
    // connect(thickAction, &QAction::triggered, this, [this](){bool ok; canvas->setStrokeWidth(QInputDialog::getDouble(this, tr("Round Rect"), tr("Enter Corner Radius:"), 10, 0, 10000, 1, &ok));});

    // undoAction = new QAction("Undo", this);
    // undoAction->setCheckable(true);
    // connect(undoAction, &QAction::triggered, this, [this](){commands.undo(); canvas->update();});

    // redoAction = new QAction("Redo", this);
    // redoAction->setCheckable(true);
    // connect(redoAction, &QAction::triggered, this, [this](){commands.redo();canvas->update();});
    // tools->addActions({borderAction, insideAction, thickAction, undoAction, redoAction});

    tools->addSeparator();
    strokeColorWidget = new QWidget();
    strokeColorWidget->setFixedSize(30, 30);
    QPalette strokePal = strokeColorWidget->palette();
    strokePal.setColor(QPalette::Window, Qt::black); // Default black
    strokeColorWidget->setPalette(strokePal);
    strokeColorWidget->setAutoFillBackground(true);

    borderAction = new QAction("Stroke", this);
    connect(borderAction, &QAction::triggered, this, [this]()
    {
        QColor color = QColorDialog::getColor(Qt::black, this, "Select Stroke", QColorDialog::ShowAlphaChannel);
        if(color.isValid())
        {
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
    fillColorWidget->setStyleSheet("background-color: white; border: 2px solid gray;");

    insideAction = new QAction("Fill", this);
    connect(insideAction, &QAction::triggered, this, [this]()
    {
        QColor color = QColorDialog::getColor(Qt::white, this, "Select Fill Color", QColorDialog::ShowAlphaChannel);
        if(color.isValid())
        {
            canvas->setcurrFill(color);
            fillColorWidget->setStyleSheet(QString("background-color: %1; border: 2px solid gray;").arg(color.name()));
        }
    });
    QWidgetAction* fillAction = new QWidgetAction(this);
    fillAction->setDefaultWidget(fillColorWidget);
    tools->addAction(fillAction);
    tools->addAction(insideAction);
    
    thickAction = new QAction("strk_width", this);
    connect(thickAction, &QAction::triggered, this, [this](){bool ok; canvas->setStrokeWidth(QInputDialog::getDouble(this, tr("Stroke Width"), tr("Enter Width:"), 1.0, 0.1, 100.0, 1, &ok));});

    undoAction = new QAction("Undo", this);
    connect(undoAction, &QAction::triggered, this, [this](){commands.undo(); canvas->update();});

    redoAction = new QAction("Redo", this);
    connect(redoAction, &QAction::triggered, this, [this](){commands.redo(); canvas->update();});

    tools->addActions({ thickAction, undoAction, redoAction});


}

void MainWindow::openSVG()
{
    QString file = QFileDialog::getOpenFileName(
        this, "Open XML", "", "XML Files (*.svg *.xml)"
    );
    if(file.isEmpty()) return;

    diagram = XMLParser::parse(file.toStdString());
    canvas->setDiagram_Cmd(&diagram, &commands);
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