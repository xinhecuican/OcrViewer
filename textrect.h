#ifndef TEXTRECT_H
#define TEXTRECT_H
#include <QGraphicsObject>
#include "utils.h"
#include <QGraphicsTextItem>
#include "textanchor.h"

class TextRect : public QGraphicsObject
{
    Q_OBJECT
public:
    TextRect(TextLine line, QGraphicsItem* parent=nullptr);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event)override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event)override;
    void setHighlighter(bool isHighlighter);
signals:
    void hovered(int index, bool isEnter);
private:
    TextLine line;
    QGraphicsTextItem* textItem;
    TextAnchor* anchor;
    bool isHover;
};

#endif // TEXTRECT_H
