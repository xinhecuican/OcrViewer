#include "listitem.h"
#include <QHBoxLayout>
#include <QLabel>
#include "utils.h"
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <QEvent>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QAction>

ListItem::ListItem(int index, QString text, QWidget* parent) : QWidget(parent) {
    setFixedWidth(270);
    setAttribute(Qt::WA_Hover, true);
    installEventFilter(this);
    this->index = index;
    this->text = text;
    QHBoxLayout* layout = new QHBoxLayout(this);
    indexLabel = new QLabel(QString::number(index), this);
    indexLabel->setMinimumWidth(20);
    textLabel = new QLabel(elidedText(this->font(), 190, 1000000, text), this);
    textLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    textLabel->setContextMenuPolicy(Qt::CustomContextMenu);
    textMenu = new QMenu(this);
    QAction* actionCopy = new QAction(tr("复制"), textMenu);
    actionCopy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    connect(actionCopy, &QAction::triggered, this, [=]() {
        QClipboard* clip = QApplication::clipboard();
        if(textLabel->selectedText() == "")
            clip->setText(textLabel->text());
        else
            clip->setText(textLabel->selectedText());
    });
    textMenu->addAction(actionCopy);
    connect(textLabel, &QLabel::customContextMenuRequested, this, [=](QPoint pos) {
        textMenu->exec(QCursor::pos());
    });
    layout->addWidget(indexLabel);
    layout->addWidget(textLabel, 1);
    connect(this, &ListItem::hover, this, &ListItem::hoverFunc);
}

void ListItem::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ListItem::hoverFunc(bool isEnter) {
    if(isEnter) {
        QPalette p;
        p.setColor(QPalette::WindowText, Qt::red);
        textLabel->setPalette(p);
    } else {
        QPalette p;
        p.setColor(QPalette::WindowText, Qt::black);
        textLabel->setPalette(p);
    }
}

bool ListItem::eventFilter(QObject* object, QEvent* event) {
    if(event->type() == QEvent::HoverEnter)
        emit hover(true);
    else if(event->type() == QEvent::HoverLeave)
        emit hover(false);
    return QWidget::eventFilter(object, event);
}

void ListItem::setHighlight(bool isHighlighter) {
    hoverFunc(isHighlighter);
}

void ListItem::setText(QString text) {
    this->text = text;
    textLabel->setText(elidedText(this->font(), 190, 1000000, text));
    update();
}

QString ListItem::getText() {
    return text;
}
