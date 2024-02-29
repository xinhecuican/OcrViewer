#ifndef LISTITEM_H
#define LISTITEM_H
#include <QWidget>
#include <QLabel>
#include <QMenu>

class ListItem : public QWidget {
    Q_OBJECT
public:
    ListItem(int index, QString text, QWidget* parent=nullptr);
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event)override;
    void setHighlight(bool isHighlighter);
    void setText(QString text);
    QString getText();
private:
signals:
    void hover(bool isEnter);
private:
    void hoverFunc(bool isEnter);
    int index;
    QString text;
    QLabel* indexLabel;
    QLabel* textLabel;
    QMenu* textMenu;
};

#endif // LISTITEM_H
