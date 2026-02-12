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
        double resizeFactor = 1.0;
        QRectF box(startPos, currentPos);
        box = box.normalized();


        // double factor = 1.0;
        // if(distCenter > 0.001) factor = (distFact / distCenter);
        if(currentTool == Tool::Resize || currentTool == Tool::Move)
        {
            double distCenter = QLineF(currShape->boundingBox().center(), justBefore).length();
            double distFact = QLineF(currShape->boundingBox().center(), currentPos).length();
            double factor = (distFact / distCenter);
            if(factor > 1.01 || factor < 0.99)
            {
                if(currentTool == Tool::Resize) currShape->resize(factor);
                else if(currentTool == Tool::Move) currShape->move(QLineF(justBefore, currentPos));
                justBefore = currentPos;
            }
            box = currShape->boundingBox();
        }

        p.setPen(QPen(Qt::DashDotLine));
        p.setBrush(Qt::NoBrush);
        p.drawRect(box);
        if(currentTool != Tool::Resize && currentTool != Tool::Move)
        {
            auto previewShape_ = ShapeMaker::create(currentTool, currStroke, currFill, currStrokeWidth);
            previewShape_->setBoundingBox(startPos, currentPos);
            previewShape_->draw(p);
        }   
    }
}

std::shared_ptr<GraphicsObject> Canvas::search(const QPointF& p_)
{
    auto objs = diagram->objects();
    
    for(auto it = objs.rbegin(); it != objs.rend(); ++it)
    {
        if(it->get()->boundingBox().contains(p_)) return *it;
    }
    return nullptr;
}

void Canvas::mousePressEvent(QMouseEvent* e)
{
    if(e->button() != Qt::LeftButton) return;

    //if tool is resizeing then scan the diagram to find current obj, if found currobj is that else return.
    if(currentTool == Tool::Resize || currentTool == Tool:: Move)
    {
        currShape = search(e->position());
    }

    dragging = true;
    startPos = e->position();
    justBefore = startPos;
    currentPos = startPos;
}

void Canvas::mouseMoveEvent(QMouseEvent* e)
{
    if(!dragging || !diagram) return;

    currentPos = e->position();
    update();
    // justBefore = currentPos;
}

void Canvas::mouseReleaseEvent(QMouseEvent* e)
{
    if(!dragging || !diagram) return;

    dragging = false;
    update();
    if(currentTool == Tool::Resize) return;
    if(currentTool == Tool::Move) return;
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
    else if(currentTool == Tool::Text)
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