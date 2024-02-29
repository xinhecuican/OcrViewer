#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "utils.h"
#include <QNetworkReply>
#include "listitem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* event)override;
    void keyReleaseEvent(QKeyEvent* event)override;
    void getTranslateText(int index, QString text);
private slots:
    void on_translateButton_clicked();

    void on_copyButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<TextLine> texts;
    QList<ListItem*> items;
    bool isHighlighter;
    int highlightIndex;
    bool controlEnter;
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply* reply;
    bool _isTranslate;
    const QString salt = "3424dfga";
    const QString appid = "20221212001494068";
    const QString password = "PYdlRC6XrkG8Kcc8cwD5";
};
#endif // MAINWINDOW_H
