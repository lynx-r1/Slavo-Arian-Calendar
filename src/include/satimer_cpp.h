/*****************************************************************************
 *   Copyright (C) 2009-2010  Jack R  <lynx21.12.12@yandex.ru>               *
 *                                                                           *
 *   This file is part of SAKalendar.                                        *
 *                                                                           *
 *   SAKalendar is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation, either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

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
