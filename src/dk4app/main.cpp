#include <QtGui/QApplication>
#include <QFontDatabase>
#include <QTextCodec>
#include <QTimer>

#include "dk4app.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);

    QFontDatabase::addApplicationFont(":/SAKalendar/DK4App/fonts/IzhitsaC.ttf");

    QCoreApplication::setApplicationName("DK4App");
    QCoreApplication::setApplicationVersion("0.1.0");
    QCoreApplication::setOrganizationDomain("sakalendar.ru");
    QCoreApplication::setOrganizationName("SAKalendar_Kalyadnik");

    DK4App w;
    QTimer::singleShot(10, &w, SLOT(show()));

    return a.exec();
}
