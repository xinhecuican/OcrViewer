#ifndef TEXTANCHOR_H
#define TEXTANCHOR_H
#include <QGraphicsObject>
#include <QFont>

class TextAnchor : public QGraphicsObject
{
public:
    TextAnchor(int index, QGraphicsItem* parent=nullptr);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    int getWidth();
private:
    qreal radius;
    int index;
    qreal diameter;
    QString indexText;
    int height;
    int width;
    QFont font;
};

#endif // TEXTANCHOR_H
