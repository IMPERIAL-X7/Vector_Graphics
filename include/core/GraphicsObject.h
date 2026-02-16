#pragma once
#include<memory>
#include<string>
#include <sstream>
#include<QPainter>
// #include "core/ShapeMaker.h"

class GraphicsObject
{
public:
    // GraphicsObject();
    virtual ~GraphicsObject(); //~GraphicsObject() = default;
    /* 
        virtual: let the derived classes, definition override 
        the current defintion of the function.
        we shouldn't write default here, because "=default" is a definition. 
        It tells the compiler to generate the body for this function here only(thereby completing the definition.)
    */

   /*
       "=0" is a declaration. It tells the compiler "there is no defintion/body here", 
       and derived classes must provide one. 
       virtual ... = 0; are called pure virtual functions this means their 
       derived must implement it(or it would be a compile error) 
       virtual ...; may have their base class defn. and if derived classes
       don't override it. then base defn. maybe called. Like: QMouseEvent, QPaintEvent, etc.
   */
    virtual void draw(QPainter& p) const = 0;
    virtual std::string toSVG() const = 0;
    virtual std::shared_ptr<GraphicsObject> clone() const = 0;
    // static std::shared_ptr<GraphicsObject> loadShape(const std::string& s1, const std::string& s2);
    //can we go >= 2 levels down in case of virtual functions?
    void resize(const double factor);
    void move(const QLineF l);
    void setStroke(const QColor& s);
    void setFill(const QColor& f);
    void setStrokeWidth(float w);
    virtual void setBoundingBox(const QPointF& p1, const QPointF& p2);
    const QRectF& boundingBox() const;

protected:
    QColor stroke = Qt::black;
    QColor fill = Qt::NoBrush;
    double strokeWidth = 1.0f;
    // QPointF p1;
    // QPointF p2;
    QRectF bBox;
};

struct style
{
    QColor stroke; double strokeWidth; QColor fill;
    style(std::string s)
    {
        size_t pos = 0;

        pos = s.find("stroke=\"") + 8;
        stroke = QColor(QString::fromStdString(s.substr(pos, s.find("\"", pos) - pos)));

        pos = s.find("fill=\"") + 6;
        fill = QColor(QString::fromStdString(s.substr(pos, s.find("\"", pos) - pos)));

        pos = s.find("stroke-width=\"") + 14;
        strokeWidth = std::stod(s.substr(pos, s.find("\"", pos) - pos));
    }
};