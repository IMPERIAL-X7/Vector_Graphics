#include "shapes/Text.h"
#include <QFontMetricsF>

Text::Text(QColor strokeColor, QColor fillColor, double strokeWidth_)
{
    stroke = strokeColor;
    fill = fillColor;
    strokeWidth = strokeWidth_;
}

void Text::draw(QPainter& p) const
{
    QPen pen(stroke);
    pen.setWidthF(strokeWidth);
    p.setPen(pen);

    if(fill != Qt::NoBrush) p.setBrush(fill);
    else p.setBrush(Qt::NoBrush);
    if (text.isEmpty() || bBox.width() <= 0 || bBox.height() <= 0) return;
    p.save();
    // const QTextF b_ = bBox;
    QFont font = p.font();
    font.setPointSizeF(100);
    QFontMetricsF fm(font);
    QRectF boundRect_ = fm.boundingRect(bBox, Qt::AlignCenter, text);
    double scaleX_ = bBox.width() / boundRect_.width();
    double scaleY_ = bBox.height() / boundRect_.height();

    double finalScale_ = std::min(scaleX_, scaleY_) * 0.95;
    font.setPointSizeF(finalScale_ * 100.0);

    p.setFont(font);
    p.drawText(bBox, Qt::AlignCenter, text);
    p.restore();
}

void Text::setText(const QString& str)
{
    text = str;
}

std::string Text::toSVG() const
{
    return "";
}

std::shared_ptr<GraphicsObject> Text::clone() const 
{
    return std::make_shared<Text>(*this);
}
