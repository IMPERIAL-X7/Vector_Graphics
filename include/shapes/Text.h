#pragma once
#include "core/GraphicsObject.h"

class Text : public GraphicsObject
{
public:
    Text(QColor strokeColor, QColor fillColor, double strokeWidth_);

    void draw(QPainter& p) const override;
    std::string toSVG() const override;
    std::shared_ptr<GraphicsObject> clone() const override;
    void setText(const QString& str);

private:
    QString text;
    double x, y, width, height;
};