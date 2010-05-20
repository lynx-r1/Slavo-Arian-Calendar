#include <QtGui/QApplication>
#include <QFontDatabase>
#include <QTextCodec>
#include <QTimer>

#include "sadbgui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);

    QFontDatabase::addApplicationFont(":/SAKalendar/SADBApp/fonts/IzhitsaC.ttf");

    QCoreApplication::setApplicationName("SADBApp");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationDomain("sakalendar.ru");
    QCoreApplication::setOrganizationName("SAKalendar_Kalyadnik");

    SADBGui w;
    QTimer::singleShot(70, &w, SLOT(show()));

    return a.exec();
}
