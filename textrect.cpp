#include "textrect.h"
#include <QPainter>
#include <QPen>

TextRect::TextRect(TextLine line, QGraphicsItem* parent) : QGraphicsObject(parent)
{
    setAcceptHoverEvents(true);
    this->line = line;
    isHover = false;
    textItem = new QGraphicsTextItem(this);
    textItem->setTextWidth(200);
    textItem->setHtml("<body bgcolor=#ffffff>"+line.text + "</body>");
    textItem->hide();
    textItem->setPos(boundingRect().bottomLeft());
    textItem->setDefaultTextColor(Qt::red);
    textItem->setTextInteractionFlags(Qt::TextSelectableByMouse);
    anchor = new TextAnchor(line.index, this);
    anchor->hide();
    anchor->setPos(boundingRect().topLeft() - QPointF(0, 20));
}

QRectF TextRect::boundingRect() const
{
    if(isHover)
    {
        QRect rect = textItem->boundingRect().toRect();
        rect.moveTo(textItem->pos().toPoint());
        return line.bound.boundingRect().united(rect);
    }
    else
        return line.bound.boundingRect();
}

QPainterPath TextRect::shape() const
{
    QPainterPath path;
//    path.addPolygon(line.bound);
//    if(isHover)
//    {
//        QPainterPath textPath = textItem->shape();
//        textPath.moveTo(textItem->pos());
//        path.addPath(textPath);
//    }
    path.addRect(boundingRect());
    return path;
}

void TextRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setColor(Qt::red);
    painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
    painter->setPen(pen);
    painter->drawPolygon(line.bound);
}

void TextRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit hovered(line.index, true);
    setZValue(1);
    textItem->show();
    anchor->show();
    isHover = true;
}

void TextRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit hovered(line.index, false);
    setZValue(0);
    textItem->hide();
    anchor->hide();
    isHover = false;
}

void TextRect::setHighlighter(bool isHighlighter)
{
    if(isHighlighter)
    {
        anchor->show();
    }
    else
    {
        anchor->hide();
    }
}
