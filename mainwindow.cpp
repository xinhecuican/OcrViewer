#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include "paintarea.h"
#include <QScrollBar>
#include <QKeyEvent>
#include <QScreen>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QClipboard>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    isHighlighter = false;
    controlEnter = false;
    _isTranslate = false;
    highlightIndex = -1;
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    ui->scrollArea->widget()->setLayout(layout);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setFrameShape(QFrame::NoFrame);
    ui->graphicsView->setBackgroundRole(QPalette::Light);
    ui->graphicsView->setAlignment(Qt::AlignCenter);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    ui->scrollArea->setBackgroundRole(QPalette::Light);
    PaintArea* area = new PaintArea(ui->graphicsView);
    area->setPix(QPixmap("ocr/1.png"));
    ui->graphicsView->setScene(area);
    ui->graphicsView->resize(area->sceneRect().width(), area->sceneRect().height());
    ui->graphicsView->horizontalScrollBar()->setValue(100);
    ui->graphicsView->verticalScrollBar()->setValue(100);
    adjustSize();
    texts = resolveFile("ocr/1.png-result.txt");
    for(int i=0; i<texts.size(); i++) {
        ListItem* listItem = new ListItem(texts[i].index, texts[i].text, this);
        connect(listItem, &ListItem::hover, this, [=](bool isEnter) {
            area->setHighlighter(i, isEnter);
            if(isEnter) {
                highlightIndex = i;
                if(isHighlighter) {
                    isHighlighter = false;
                    items[highlightIndex]->setHighlight(false);
                }
            }
        });
        items.append(listItem);
        layout->addWidget(listItem);
    }
    area->setRect(texts);
    connect(area, &PaintArea::hovered, this, [=](int index, bool isEnter) {
        if(isEnter) {
            int height = 0;
            for(int i=0; i<index; i++)
                height += items[i]->height();
            ui->scrollArea->verticalScrollBar()->setSliderPosition(height);
            isHighlighter = true;
            highlightIndex = index;
            items[index]->setHighlight(true);
        } else {
            isHighlighter = false;
            items[index]->setHighlight(false);
        }
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Control)
        controlEnter = true;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Control)
        controlEnter = false;
}

void MainWindow::on_translateButton_clicked() {
    if(!_isTranslate) {
        for(int i=0; i<items.size(); i++) {
            getTranslateText(i, items[i]->getText());
        }
    }
    _isTranslate = true;
}

void MainWindow::on_copyButton_clicked() {
    if(highlightIndex != -1) {
        QClipboard *clip=QApplication::clipboard();
        clip->setText(items[highlightIndex]->getText());
    }
}

void MainWindow::getTranslateText(int index, QString text) {
    QString sign = appid + text + salt + password;
    QString signMD5;
    QByteArray ba,bb;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba.append(sign);
    md.addData(ba);
    bb = md.result();
    signMD5.append(bb.toHex());
    QString url = QString("https://fanyi-api.baidu.com/api/trans/vip/translate?q=%1&from=en&to=zh&appid=%2&salt=%3&sign=%4").arg(text).arg(appid).arg(salt).arg(signMD5);
    request.setUrl(QUrl(url));
    QNetworkReply* reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {

        if(reply->error()!=QNetworkReply::NoError) {
            qWarning() << reply->errorString();
        } else {

            const QByteArray replyData =reply->readAll();
            QJsonParseError json_error;
            QJsonDocument document=QJsonDocument::fromJson(replyData, &json_error);
            if(json_error.error==QJsonParseError::NoError) {
                if(document.isObject()) {
                    const QJsonObject obj=document.object();
                    if(obj.contains("trans_result")) {
                        QJsonArray value=obj.value("trans_result").toArray();
                        QJsonObject translateObject = value.at(0).toObject();
                        QString src = translateObject.value("src").toString();
                        QString dst = translateObject.value("dst").toString();
                        items[index]->setText(src + "\n" + dst);
                    }
                }
            } else {
                qDebug()<<"json error:"<<json_error.errorString();
            }
        }
        reply->deleteLater();
    });
}
