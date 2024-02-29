#include "paintarea.h"
#include <QDebug>
#include "textrect.h"
#include <QGraphicsView>
#include <QGraphicsSceneWheelEvent>
#include <QScrollBar>

PaintArea::PaintArea(QObject* parent) : QGraphicsScene(parent)
{
    pixItem = new QGraphicsPixmapItem();
    addItem(pixItem);
    this->parent = qobject_cast<QGraphicsView*>(parent);
}

void PaintArea::setPix(QPixmap pix)
{
    setSceneRect(QRectF(-100, -100, pix.width()+200, pix.height()+200));
    pixItem->setPixmap(pix);
}

void PaintArea::setRect(QList<TextLine> lines)
{
    for(TextLine line : lines)
    {
        TextRect* textRect = new TextRect(line);
        connect(textRect, &TextRect::hovered, this, [=](int index, bool isEnter){
            emit hovered(index, isEnter);
        });
        addItem(textRect);
        rects.append(textRect);
    }
}

void PaintArea::setHighlighter(int index, bool isHighlighter)
{
    rects[index]->setHighlighter(isHighlighter);
    parent->centerOn(rects[index]);
}
