#include "satime_c.h"
#include "satimer_cpp.h"

SATimer::SATimer(QObject *parent)
    : QObject(parent), mPause(false)
{
}

void SATimer::start(int interval, int dolya, int chast, int chas)
{
    mInterval = interval;
    mDolya = dolya, mChast = chast, mChas = chas;

    // обновлять нужно каждые 28.93518518518518518518 милисекунды, но так как
    // в таймере можно задать только целые значение, то округляем время обновления.
    // 29 милисекунд слишком мало, поэтому умножаем частоту на интервал прохода
    mTimerId = startTimer(29 * mInterval);
}

void SATimer::stop()
{
    mInterval = 0;
    mDolya = mChast = mChas = 0;
    killTimer(mTimerId);
}

void SATimer::pause()
{
    mPause = true;
}

void SATimer::contin()
{
    mPause = false;
}

bool SATimer::paused()
{
    return mPause;
}

QString SATimer::time()
{
    return QString("%1:%2:%3").arg(mChas).arg(mChast).arg(mDolya);
}

void SATimer::timerEvent(QTimerEvent *)
{
    mDolya += mInterval;

    if (mDolya < 1296 && !mPause)
        emit dolyaChanged(mDolya);

    if (mDolya >= 1296) {
        mDolya = mInterval;
        if (!mPause)
            emit dolyaChanged(mInterval);

        mChast++;
        if (mChast < 144 && !mPause)
            emit chastChanged(mChast);

        if (mChast == 144) {
            mChast = 1;
            if (!mPause)
                emit chastChanged(1);

            mChas++;
            if (mChas <= 16 && !mPause)
                emit chasChanged(mChas);

            if (mChas > 16) {
                mChas = 1;
                if (!mPause)
                    emit chasChanged(1);

                mDen++;
                int dni = sa_svyashch_leto(mLetoOtSMZH) ? 41 : mSorokovnik % 2 ? 41 : 40;
                if (mDen <= dni && !mPause)
                    emit denChanged(mDen);

                if (mDen > dni) {
                    mDen = 1;
                    if (!mPause)
                        emit denChanged(1);

                    mSorokovnik++;
                    if (mSorokovnik <= 9 && !mPause)
                        emit sorokovnikChanged(mSorokovnik);

                    if (mSorokovnik > 9) {
                        mSorokovnik = 1;
                        if (!mPause)
                            emit sorokovnikChanged(1);

                        mLetoOtSMZH++;

                        mLvKLet = sa_lvklet(mLetoOtSMZH);
                        mLvKZhizni = sa_lvkzhizni(mLetoOtSMZH);
                    }
                }
            }
        }
    }
}
