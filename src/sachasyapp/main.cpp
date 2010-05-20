#include <QtGui/QApplication>
#include <QFontDatabase>
#include <QTextCodec>

#include <QGraphicsEffect>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "sachasyapp.h"

#include <QLayout>
#include <QLabel>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);

    QFontDatabase::addApplicationFont(":/SAKalendar/SAChasyApp/fonts/IzhitsaC.ttf");

    QCoreApplication::setApplicationName("SAChasyApp");
    QCoreApplication::setApplicationVersion("0.1.0");
    QCoreApplication::addLibraryPath("../lib");
    QCoreApplication::setOrganizationDomain("sakalendar.ru");
    QCoreApplication::setOrganizationName("SAKalendar_Kalyadnik");

    SAChasyApp sachasy;
    sachasy.show();

    return a.exec();
}
