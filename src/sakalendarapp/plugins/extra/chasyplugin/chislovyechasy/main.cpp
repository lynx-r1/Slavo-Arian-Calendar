#include <QtGui/QApplication>
#include <QTextCodec>

#include "chislovyechasy.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    int chas = 4;
    ChislovyeChasy c(chas, chas, chas, true);
    c.show();
    return a.exec();
}
