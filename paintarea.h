#ifndef PAINTAREA_H
#define PAINTAREA_H
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "utils.h"
#include "textrect.h"

class PaintArea : public QGraphicsScene
{
    Q_OBJECT
public:
    PaintArea(QObject* parent=nullptr);
    void setPix(QPixmap pix);
    void setRect(QList<TextLine> lines);
    void setHighlighter(int index, bool isHighlighter);
signals:
    void hovered(int index, bool isEnter);
private:
    QGraphicsView* parent;
    QGraphicsPixmapItem* pixItem;
    QList<TextRect*> rects;
};

#endif // PAINTAREA_H
