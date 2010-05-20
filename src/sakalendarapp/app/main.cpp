#include <QtGui/QApplication>
#include <QFontDatabase>
#include <QTextCodec>

#include "sakalendar.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // устанавливаем UTF-8 кодек для корректного отображения русских букв
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);

    QFontDatabase::addApplicationFont(":/SAKalendar/SAKalendarApp/fonts/IzhitsaC.ttf");

    QCoreApplication::setApplicationName("SAKalendarApp");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationDomain("sakalendar.ru");
    QCoreApplication::setOrganizationName("SAKalendar_Kalyadnik");

    SAKalendar w;
    w.show();
    return a.exec();
}
