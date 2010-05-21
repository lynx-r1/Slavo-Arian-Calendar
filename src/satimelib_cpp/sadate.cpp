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

#include "satime_c.h"
#include "sadate_cpp.h"

SADate::SADate(int leto, int sorok, int den)
{
    setDate(leto, sorok, den);
}

int SADate::daysInSorok() const
{
    return mSorokovnik % 2 == 0 ? 40 : 41;
}

int SADate::daysInLete() const
{
    return sa_svyashch_leto(mLeto) ? 369 : 365;
}

int SADate::lvkzhizni(int leto)
{
    if (leto < 0)
        leto = 1;

    return sa_lvkzhizni(leto);
}

int SADate::lvklet(int leto)
{
    if (leto < 0)
        leto = 1;

    return sa_lvklet(leto);
}

int SADate::kzhizniFromSMZH(int leto)
{
    return sa_kzhizni_ot_smzh(leto);
}

int SADate::letoFromSMZH(int kzhizniFromSMZH, int lvkzhizni)
{
    return sa_leto_ot_smzh(kzhizniFromSMZH, lvkzhizni);
}

int SADate::stikhiya(int leto)
{
    if (leto < 0)
        leto = 1;

    return sa_stikhiya(leto);
}

void SADate::getDate(int *leto, int *sorok, int *den)
{
    *leto = mLeto;
    *sorok = mSorokovnik;
    *den = mDay;
}

bool SADate::setDate(int leto, int sorok, int den)
{
    if (isValid(sorok, den)) {
        mLeto = leto;
        mSorokovnik = sorok;
        mDay = den;

        sa_tm *tm = saTime();
        tm->tm_leto = mLeto;
        tm->tm_sorokovnik = mSorokovnik;
        tm->tm_dens = mDay;
        sa_mktime(tm);

        mDayOfWeek = tm->tm_denn;
        mDayOfLeta = tm->tm_denl;
        mKLet = tm->tm_lvklet;
        mKZhizni = tm->tm_lvkzhizni;
        mKZhizniFromSMZH = tm->tm_kzhizni_ot_smzh;

        mNachRamkhata = tm->tm_nachr;
        mIsDST = tm->tm_zimvr;
        mIsSvyashchLeto = tm->tm_svya;

        return true;
    } else {
        return false;
    }
}

bool SADate::isValid() const
{
    return isValid(mSorokovnik, mDay);
}

bool SADate::capLetters(const QString &str, const QChar &ch, int pos, int count) const
{
    int i = pos;
    while (i < str.size() && i < pos + count) {
        if (ch != str.at(i))
            return false;
        ++i;
    }
    return true;
}

// TODO: сделать более продуманную реализацию
QString SADate::toString(const QString &format) const
{
    if (format.isEmpty())
        return QString();

    QString date;
    int i = 0;
    while (i < format.size()) {
        if ("d" == format.at(i)) {
            if (capLetters(format, 'd', i + 1, 3)) { // dddd
                QString dddd = longDayName(mDayOfWeek);
                date += dddd;
                i += 4;
            } else if (capLetters(format, 'd', i + 1, 2)) { // ddd
                QString ddd = shortDayName(mDayOfWeek);
                date += ddd;
                i += 3;
            } else if (capLetters(format, 'd', i + 1, 1)) { // dd
                QString dd = mDay < 10 ? QString("0%1").arg(mDay) : QString::number(mDay);
                date += dd;
                i += 2;
            } else {
                date += QString::number(mDay);
                ++i;
            }
        } else if ("s" == format.at(i)) {
            if (capLetters(format, 's', i + 1, 3)) { // ssss
                date += longSorokName(mSorokovnik);
                i += 4;
            } else if (capLetters(format, 's', i + 1, 2)) { // sss
                date += shortSorokName(mSorokovnik);
                i += 3;
            } else if (capLetters(format, 's', i + 1, 1)) { // ss
                QString ss = mSorokovnik < 10 ? QString("0%1").arg(mSorokovnik)
                    : QString::number(mSorokovnik);
                date += ss;
                i += 2;
            } else {
                date += QString::number(mSorokovnik);
                ++i;
            }
        } else if ("l" == format.at(i)) {
            if (capLetters(format, 'l', i + 1, 3)) { // llll
                date += QString::number(mLeto);
                i += 4;
            } else if (capLetters(format, 'l', i + 1, 1)) { // ll
                date += QString::number(mLeto % 100);
                i += 2;
            }
        } else {
            date += format.at(i);
            ++i;
        }
    }

    return date;
}

SADate SADate::currentDate()
{
    sa_tm *tm = saTime();
    return SADate(tm->tm_leto, tm->tm_sorokovnik, tm->tm_dens);
}

int SADate::nachRamkhata(int leto)
{
    return SADate(leto, 1, 1).nachRamkhata();
}

bool SADate::isValid(int sorok, int den)
{
    return (sorok > 0 && sorok <= 9) && den > 0
            && (sorok % 2 == 0 ? den <= 40 : den <= 41);
}

bool SADate::isSvyashchLeto(int leto)
{
    if (leto < 0) {
        leto = 1;
    }
    return sa_svyashch_leto(leto);
}

QString SADate::longDayName(int weekday)
{
    if (weekday < 1 || weekday > 9) {
        weekday = 1;
    }
    return QString(sa_day_name(weekday));
}

QString SADate::longSorokName(int sorok)
{
    if (sorok < 1 || sorok > 9) {
        sorok = 1;
    }
    return QString(sa_sorokovnik_name(sorok));
}

QString SADate::shortDayName(int weekday)
{
    return longDayName(weekday).left(3);
}

QString SADate::shortSorokName(int sorok)
{
    return longSorokName(sorok).left(3);
}

sa_tm *SADate::saTime()
{
    sa_timef_t t = sa_time(NULL);
    sa_tm *tm = new sa_tm;
    tm = sa_localtime(&t);
    return tm;
}
