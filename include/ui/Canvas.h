#pragma once
#include "core/Diagram.h"
#include "core/Tool.h"
#include <QWidget>
#include <QPointF>
#include <QMouseEvent>
#include <QPaintEvent>

class Canvas : public QWidget 
{
    Q_OBJECT 
    /*
        whenever using any Qt feature like: signals, slots,
        connect, QObject, etc. this needs to be done.
        In the backend when Qt builds your project, 
        it runs a tool called moc (Meta-Object Compiler) Q_OBJECT tells
        moc to generate extra code for this class.
    */
public:
    explicit Canvas(QWidget* parent = nullptr);
    ~Canvas();
    /*
        explicit is used to prevent defn. like:
        QWidget* w = ...;
        Canvas c = w; 
        and encourage 
        Canvas(w);
    */
    void setDiagram(Diagram* d);
    void setTool(Tool t);
    void setcurrStroke(QColor c);
    void setcurrFill(QColor c);
    void setStrokeWidth(double d);
    // void move(QLineF l);
    std::shared_ptr<GraphicsObject> search(const QPointF& p_);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

private:
    Diagram* diagram = nullptr;
    Tool currentTool = Tool::None;
    std::shared_ptr<GraphicsObject> currShape = nullptr;
    bool dragging = false;
    QPointF startPos;
    QPointF justBefore;
    QPointF currentPos;
    QColor currStroke = Qt::black;
    QColor currFill = Qt::NoBrush;
    double currStrokeWidth = 1.0f;
};