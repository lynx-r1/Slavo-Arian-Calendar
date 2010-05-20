#include <QtGui/QApplication>
#include <QFontDatabase>
#include <QTextCodec>
#include <QTimer>

#include "sorokovnikapp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);

    QFontDatabase::addApplicationFont(":/SAKalendar/SorokovnikApp/fonts/IzhitsaC.ttf");

    QCoreApplication::setApplicationName("SorokovnikApp");
    QCoreApplication::setApplicationVersion("0.1.0");
    QCoreApplication::setOrganizationDomain("sakalendar.ru");
    QCoreApplication::setOrganizationName("SAKalendar_Kalyadnik");

    SorokovnikApp w;
    QTimer::singleShot(70, &w, SLOT(show()));

    return a.exec();
}
