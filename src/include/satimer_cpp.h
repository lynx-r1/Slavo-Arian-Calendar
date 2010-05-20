#ifndef KDTIMETIMER_H
#define KDTIMETIMER_H

#include <QObject>

class SATimer : public QObject
{
    Q_OBJECT

public:
    explicit SATimer(QObject *parent = 0);

    void start(int interval, int dolya, int chast, int chas);
    void stop();
    void pause();
    void contin();
    bool paused();

    QString time();
    int interval() { return mInterval; }

signals:
    void dolyaChanged(int dolya);
    void chastChanged(int chast);
    void chasChanged(int chas);

    void denChanged(int den);
    void sorokovnikChanged(int sorokovnik);
    void kletChanged(int klet);
    void kzhizniChanged(int kzhizni);
    void letoChanged(int leto);

protected:
    void timerEvent(QTimerEvent *);

private:
    int mTimerId;

    int mInterval;

    int mDolya;
    int mChast;
    int mChas;

    int mDen;
    int mSorokovnik;
    int mLvKLet;
    int mLvKZhizni;
    int mLetoOtSMZH;

    bool mPause;
};

#endif // KDTIMETIMER_H
