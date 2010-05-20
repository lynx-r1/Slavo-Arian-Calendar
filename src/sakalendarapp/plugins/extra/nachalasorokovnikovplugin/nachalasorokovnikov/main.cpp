#include <QtGui/QApplication>
#include <QTabWidget>
#include <QTextCodec>
#include <QGraphicsView>
#include <QPushButton>

#include "kd.h"
#include "nachalasorokovnikovokno.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QGraphicsWidget *nachSorok = new NachalaSorokovnikovOkno;
    QGraphicsScene s;
    s.addItem(nachSorok);

    QGraphicsView v(&s);
    v.resize(800, 500);
    v.show();

    return a.exec();
}
