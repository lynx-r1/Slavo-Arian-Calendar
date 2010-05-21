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
#include "satime_cpp.h"

SATime::SATime(int chas, int chast, int dolya)
    : mChas(chas)
    , mChast(chast)
    , mDolya(dolya)
{
    mIsValid = isValid();
}

bool SATime::isValid() const
{
    return isValid(mChas, mChast, mDolya);
}

QString SATime::chasText(int chas)
{
    QString chasText = QString::number(chas);

    if (chasText.length() < 2)
        chasText.prepend("0");
    return chasText;
}

QString SATime::chastText(int chast)
{
    QString chastText = QString::number(chast);

    if (chastText.length() < 2)
        chastText.prepend("00");
    else if (chastText.length() < 3)
        chastText.prepend("0");
    return chastText;
}

QString SATime::dolyaText(int dolya)
{
    QString dolyaText = QString::number(dolya);

    if (dolyaText.length() < 2)
        dolyaText.prepend("000");
    else if (dolyaText.length() < 3)
        dolyaText.prepend("00");
    else if (dolyaText.length() < 4)
        dolyaText.prepend("0");
    return dolyaText;
}

bool SATime::setCCD(int chas, int chast, int dolya)
{
    if (isValid(chas, chast, dolya)) {
        mChas = chas;
        mChast = chast;
        mDolya = dolya;
        return true;
    } else {
        return false;
    }
}

bool SATime::capLetters(const QString &str, const QChar &ch, int pos, int count) const
{
    int i = pos;
    while (i < str.size() && i < pos + count) {
        if (ch != str.at(i))
            return false;
        ++i;
    }
    return true;
}

QString SATime::toString(const QString &format) const
{
    if (format.isEmpty())
        return QString();

    QString time;
    int i = 0;
    while (i < format.size()) {
        if ("c" == format.at(i)) {
            if (capLetters(format, 'c', i + 1, 1)) { // cc
                QString cc = mChas < 10 ? QString("0%1").arg(mChas) : QString::number(mChas);
                time += cc;
                i += 2;
            } else { // c
                time += QString::number(mChas);
                ++i;
            }
        } else if ("C" == format.at(i)) {
            if (capLetters(format, 'C', i + 1, 2)) { // CCC
                QString CCC = QString::number(mChast);
                if (CCC.length() < 2)
                    CCC.prepend("00");
                else if (CCC.length() < 3)
                    CCC.prepend("0");
                time += CCC;
                i += 3;
            } else { // C
                time += QString::number(mChast);
                ++i;
            }
        } else if ("D" == format.at(i)) {
            if (capLetters(format, 'D', i + 1, 3)) { // DDDD
                QString DDDD = QString::number(mDolya);
                if (DDDD.length() < 2)
                    DDDD.prepend("000");
                else if (DDDD.length() < 3)
                    DDDD.prepend("00");
                else if (DDDD.length() < 4)
                    DDDD.prepend("0");
                time += DDDD;
                i += 4;
            } else { // D
                time += QString::number(mDolya);
                ++i;
            }
        } else {
            time += format.at(i);
            ++i;
        }
    }

    return time;
}

SATime SATime::currentTime(bool ignoreDST)
{
    sa_ignore_dst = ignoreDST;

    sa_timef_t t = sa_time(NULL);
    sa_tm *tm = sa_localtime(&t);
    return SATime(tm->tm_chas, tm->tm_chast, tm->tm_dolya);
}

bool SATime::isValid(int chas, int chast, int dolya)
{
    bool valid;
    if (chas >= 1 && chas <= 16
        && chast >= 0 && chast < 144
        && dolya >= 0 && dolya < 1296) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}
