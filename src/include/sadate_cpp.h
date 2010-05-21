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

#ifndef KDDATE_H
#define KDDATE_H

#include <QString>

class SADate
{
public:
    SADate() : mIsValid(false) {}
    SADate(int leto, int sorok, int den);

    inline int day() const { return mDay; }
    inline int sorokovnik() const { return mSorokovnik; }
    inline int leto() const { return mLeto; }
    inline int lvklet() const { return mKLet; }
    inline int lvkzhizni() const { return mKZhizni; }
    inline int kzhizniFromSMZH() const { return mKZhizniFromSMZH; }
    inline int dayOfWeek() const { return mDayOfWeek; }
    inline int dayOfLeta() const { return mDayOfLeta; }
    inline int nachRamkhata() const { return mNachRamkhata; }

    inline int isDST() const { return mIsDST; }
    inline bool isSvyashchLeto() const { return mIsSvyashchLeto; }

    int daysInSorok() const;
    int daysInLete() const;

    void getDate(int *leto, int *sorok, int *den);
    bool setDate(int leto, int sorok, int den);

    bool isValid() const;

    QString toString(const QString &format) const;

    static SADate currentDate();

    static int nachRamkhata(int leto);
    static int lvkzhizni(int leto);
    static int lvklet(int leto);
    static int kzhizniFromSMZH(int leto);
    static int letoFromSMZH(int kzhizniFromSMZH, int lvkzhizni);
    static int stikhiya(int leto);

    static bool isValid(int sorok, int den);
    static bool isSvyashchLeto(int leto);

    static QString longDayName(int weekday);
    static QString longSorokName(int sorok);
    static QString shortDayName(int weekday);
    static QString shortSorokName(int sorok);

private:
    bool capLetters(const QString &str, const QChar &ch, int pos, int count) const;

    static struct sa_tm *saTime();

private:
    int mLeto;
    int mSorokovnik;
    int mDay;
    int mDayOfWeek;
    int mDayOfLeta;
    int mKLet;
    int mKZhizni;
    int mKZhizniFromSMZH;
    int mNachRamkhata;
    int mIsDST;

    bool mIsSvyashchLeto;
    bool mIsValid;
};

#endif // KDDATE_H
