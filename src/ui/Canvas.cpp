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

void Canvas::setDiagram_Cmd(Diagram* d, Command* cmd)
{
    diagram = d;
    commandPlate = cmd;
    update(); //update tells Qt smthng had changed please redraw.
}

void Canvas::setTool(Tool t)
{
    currentTool = t;
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

void Canvas::paintEvent(QPaintEvent*)
{
    if(!diagram) return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    //Antialiasing to smoothen the curve, remove staircase effect.
    if(currentTool == Tool::Select)
    {
        p.setPen(QPen(Qt::red, 2, Qt::DashDotDotLine));
        p.drawRect(currShape->boundingBox());
    }

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

        p.setPen(QPen(Qt::red, 2, Qt::DashDotDotLine));
        p.setBrush(Qt::NoBrush);
        p.drawRect(box);
        if(currentTool != Tool::Resize && currentTool != Tool::Move)
        {
            auto previewShape_ = ShapeMaker::create(currentTool, currStroke, currFill, currStrokeWidth);
            std::cout << "Preview shape created: " << (previewShape_ ? "Valid" : "Null") << std::endl;
            previewShape_->setBoundingBox(startPos, currentPos);
            std::cout << "Drawing preview shape" << std::endl;
            previewShape_->draw(p);
        }   
    }
}

void Canvas::mouseDoubleClickEvent(QMouseEvent* e)
{
    currentTool = Tool::Move;
    currShape = search(e->position());update();
    dragging = true;
    startPos = e->position();
    justBefore = startPos;
    currentPos = startPos;
}

void Canvas::mousePressEvent(QMouseEvent* e)
{
    if(e->button() == Qt::RightButton)
    {
        currentTool = Tool::Resize;
        currShape = search(e->position());update();
        dragging = true;
        startPos = e->position();
        justBefore = startPos;
        currentPos = startPos; return;
    }
    if(e->button() != Qt::LeftButton) return;
    if(currentTool == Tool::Select)
    {
        currShape = search(e->position()); 
        update(); return;
    }
    commandPlate->clearRedoStack();
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
    currentPos = e->position();
    if(currentTool == Tool::Resize) return;
    if(currentTool == Tool::Move)
    {
        commandPlate->undo_push({2, currShape, QLineF(startPos, currentPos)}); return;
    }
    
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
    commandPlate->undo_push({0, shape, QLineF(startPos, currentPos)});
    if(currentTool == Tool::Circle) 
    {
        auto s = std::dynamic_pointer_cast<Circle>(shape); s->resizeBoundingBox();
    }
    if(currentTool == Tool::Hexagon) 
    {
        auto s = std::dynamic_pointer_cast<Hexagon>(shape); s->resizeBoundingBox();
    }
    update();
    startPos = currentPos;
}

void Canvas::copy()
{
    copiedShape = currShape->clone(); update();
}

void Canvas::cut()
{
    copy(); diagram->destroy(currShape); update();
}

void Canvas::paste()
{
    copiedShape->move(QLineF(0.0, 0.0, 5.0, 5.0));
    diagram->add(copiedShape); update();
}