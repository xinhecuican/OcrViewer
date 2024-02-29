#ifndef UTILS_H
#define UTILS_H
#include <QFile>
#include <QRect>
#include <QDebug>
#include <QDir>
#include <QRegExp>
#include <QString>
#include <QPolygon>
#include <QFont>
#include <QFontMetrics>
#include <QNetworkAccessManager>

static QString getQSS(QString path) {
    QFile file(path);
    file.open(QFile::ReadOnly);
    QString str = file.readAll();
    file.close();
    return str;
}

struct TextLine {
    int index;
    QString text;
    QPolygon bound;
};

static QList<TextLine> resolveFile(QString fileName) {
    QDir dir;
    QFile file(dir.absoluteFilePath(fileName));
    QList<TextLine> lines;
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "file open error";
        return lines;
    }
    QString content = file.readAll();
    QRegExp boxReg("TextBox\\[(\\d+)\\]\\(\\+padding\\)\\[score\\(\\d*\\.\\d+\\),\\[x: (\\d+), y: (\\d+)\\], \\[x: (\\d+), y: (\\d+)\\], \\[x: (\\d+), y: (\\d+)\\], \\[x: (\\d+), y: (\\d+)\\]\\]");
    int pos = 0;
    int matchLength = 0;
    while((pos = boxReg.indexIn(content, pos+matchLength)) != -1) {
        matchLength = boxReg.matchedLength();
        TextLine line;
        line.index = boxReg.cap(1).toInt();
        line.bound.append(QPoint(boxReg.cap(2).toInt()-50, boxReg.cap(3).toInt()-50));
        line.bound.append(QPoint(boxReg.cap(4).toInt()-50, boxReg.cap(5).toInt()-50));
        line.bound.append(QPoint(boxReg.cap(6).toInt()-50, boxReg.cap(7).toInt()-50));
        line.bound.append(QPoint(boxReg.cap(8).toInt()-50, boxReg.cap(9).toInt()-50));
        lines.append(line);
    }
    QRegExp textReg("FullDetectTime");
    pos = 0;
    matchLength = 0;
    pos = textReg.indexIn(content, pos+matchLength);
    for(int i=pos; i<content.size() - 1; i++) {
        if(content[i] == '\xa') {
            pos = i+1;
            break;
        }
    }

    int index = 0;
    for(int i=pos; i<content.size()-1; i++) {
        if(content[i] == '\r')
            continue;
        if(content[i] == '\xa') {
            index++;
            continue;
        } else {
            lines[index].text.append(content[i]);
        }
    }
    return lines;
}

static int calcStringWidth(QFont font, QString text) {
    int ans = 0;
    QFontMetrics fontMetrics(font);
    for(int i=0; i<text.size(); i++) {
        ans += fontMetrics.horizontalAdvance(text[i]);
    }
    return ans;
}

static QString elidedText(QFont font, int width, int line, QString text) {
    if(line == 0)
        return "";

    QFontMetrics fontMetrics(font);

    if(line == 1) {
        return fontMetrics.elidedText(text, Qt::ElideMiddle, width);
    }

    QStringList strListLine;
    for(int i = 0; i < text.size(); i++) {
        if(fontMetrics.horizontalAdvance(text.left(i)) >= width) {
            strListLine.append(text.left(i));
            text = text.right(text.size() - i);
            i = 0;
        }
    }
    //最后不足一行的也加进去
    if(!text.isEmpty()) {
        strListLine.append(text);
    }

    //超过指定行数 从倒数第二行开始删除
    bool bHasElided = false; //标记是否有省略内容
    while(strListLine.count() > line) {
        if(strListLine.count() >= 2) {
            strListLine.removeAt(strListLine.count()-2);
            bHasElided = true;
        }
    }

    //最后一行加了...超行  就从省略号后开始删除字符 直到不超为止
    if(bHasElided && !strListLine.isEmpty()) {
        QString strLast = strListLine.last();
        QString strElided = "...";
        strLast.insert(0,strElided);
        while(fontMetrics.horizontalAdvance(strLast) >= width) {
            strLast = strLast.replace(strElided.size(),1,"");
        }

        strListLine.replace(strListLine.count() - 1,strLast);
    }
    QString strResult = strListLine.join("\n");

    return strResult;
}
#endif // UTILS_H
