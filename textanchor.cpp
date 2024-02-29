#include "textanchor.h"
#include <QPainter>
#include "utils.h"

TextAnchor::TextAnchor(int index, QGraphicsItem* parent) : QGraphicsObject(parent)
{
    this->index = index;
    indexText = QString::number(index);
    font.setWeight(2);
    font.setPointSize(10);
    width = calcStringWidth(font, indexText);
    QFontMetrics fontMetrics(font);
    height = fontMetrics.height();
}

QRectF TextAnchor::boundingRect() const
{
    return QRectF(0, 0, width+6, height+4);
}

QPainterPath TextAnchor::shape() const
{
    QPainterPath path;
    path.addEllipse(0, 0, width+6, height+4);
    return path;
}

void TextAnchor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setColor(Qt::white);
    painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
    painter->setPen(pen);
    painter->setFont(font);
    painter->fillPath(shape(), Qt::red);
    painter->drawText(QRectF(3, 2, width, height), indexText);
}
