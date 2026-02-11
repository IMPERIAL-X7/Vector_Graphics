#include "ui/Canvas.h"
#include "shapes/Circle.h"
#include "core/ShapeMaker.h"
#include <QPainter>
#include <iostream>
#include <QMouseEvent>
#include <QInputDialog>

Canvas::Canvas(QWidget* parent) : QWidget(parent)
{
    resize(600, 600);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
    setAutoFillBackground(true);
}
Canvas::~Canvas() {}

void Canvas::setcurrStroke(QColor c){currStroke = c;}
void Canvas::setcurrFill(QColor c) {currFill = c;}
void Canvas::setStrokeWidth(double d) {currStrokeWidth = d;}

void Canvas::setDiagram(Diagram* d)
{
    diagram = d;
    update(); //update tells Qt smthng had changed please redraw.
}

void Canvas::setTool(Tool t)
{
    currentTool = t;
}

void Canvas::paintEvent(QPaintEvent*)
{
    if(!diagram) return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    //Antialiasing to smoothen the curve, remove staircase effect.

    for(const auto& obj : diagram->objects())
    {
        obj->draw(p);
    }

    if(dragging)
    {
        QRectF box(startPos, currentPos);
        box = box.normalized();

        p.setPen(QPen(Qt::DashDotLine));
        p.setBrush(Qt::NoBrush);
        p.drawRect(box);

        auto previewShape_ = ShapeMaker::create(currentTool, currStroke, currFill, currStrokeWidth);
        previewShape_->setBoundingBox(startPos, currentPos);
        previewShape_->draw(p);
    }
}

void Canvas::mousePressEvent(QMouseEvent* e)
{
    if(e->button() != Qt::LeftButton) return;

    dragging = true;
    startPos = e->position();
    currentPos = startPos;
}

void Canvas::mouseMoveEvent(QMouseEvent* e)
{
    if(!dragging || !diagram) return;

    currentPos = e->position();
    update();
}

void Canvas::mouseReleaseEvent(QMouseEvent* e)
{
    if(!dragging || !diagram) return;

    dragging = false;
    currentPos = e->position();

    auto shape = ShapeMaker::create(currentTool, currStroke, currFill, currStrokeWidth);
    if(!shape) return;

    shape->setBoundingBox(startPos, currentPos);

    if(currentTool == Tool::RoundRect)
    {
        bool ok; 
        float radius = QInputDialog::getDouble(this, tr("Round Rect"), tr("Enter Corner Radius:"), 10, 0, 10000, 1, &ok);
        if(ok)
        {
            auto rr = std::dynamic_pointer_cast<RoundRect>(shape);
            rr->setRadius(radius);
            std::cout << rr->viewRadius() << std::endl;
        }
        else 
        {
            update(); return;
        }
    }
    if(currentTool == Tool::Text)
    {
        bool ok;
        QString qText = QInputDialog::getText(this, tr("Text Box"), tr("Enter text"), QLineEdit::Normal, tr("write here"), &ok);
        if(ok)
        {
            auto textBox = std::dynamic_pointer_cast<Text>(shape); textBox->setText(qText);
        }
        else {update(); return;}
    }
    diagram->add(shape);
    update();
    startPos = currentPos;
}