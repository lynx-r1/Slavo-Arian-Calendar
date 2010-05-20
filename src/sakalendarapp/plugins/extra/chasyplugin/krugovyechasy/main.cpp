#include <QtGui/QApplication>
#include <QTextCodec>

#include "satime.h"
#include "kdtimetimer.h"

#include "krugovyechasy.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    sa_timef_t sa_t = sa_time((sa_timef_t *)NULL);
    sa_tm *tm = sa_localtime(&sa_t);
    KDTimeTimer *timer = new KDTimeTimer();
    timer->start(4, tm->tm_dolya, tm->tm_chast, tm->tm_chas);

    KrugovyeChasy k(timer, true);
    k.show();
    return a.exec();
}
