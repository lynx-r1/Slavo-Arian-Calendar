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

#ifndef KDTIME_H
#define KDTIME_H

#include <QString>

class SATime
{
public:
    SATime() : mIsValid(false)
    {}
    SATime(int chas, int chast, int dolya = 0);

    inline int dolya() const { return mDolya; }
    inline int chast() const { return mChast; }
    inline int chas() const { return mChas; }

    bool isValid() const;

    bool setCCD(int chas, int chast, int dolya);

    /*!
      с - час без нуля впереди (0 - 16)
      сc - час с нулём впереди (00 - 16)
      С - часть без нулей впереди (0 - 144)
      ССС - часть с нулями впереди (000 - 144)
      D - доля без нулей впереди (0 - 1296)
      DDDD -доля с нулями впереди (0000 - 1296)
    */
    QString toString(const QString &format) const;

    static SATime currentTime(bool ignoreDST = false);
    static bool isValid(int chas, int chast, int dolya);

    static QString chasText(int chas);
    static QString chastText(int chast);
    static QString dolyaText(int dolya);

private:
    bool capLetters(const QString &str, const QChar &ch, int pos, int count) const;

private:
    int mChas;
    int mChast;
    int mDolya;

    bool mIsValid;
};

#endif // KDTIME_H
