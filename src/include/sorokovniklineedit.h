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

#ifndef SOROKOVNIKLINEEDIT_H
#define SOROKOVNIKLINEEDIT_H

#include <QLineEdit>
#include <QMap>

class SorokovnikLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    SorokovnikLineEdit(QWidget *parent = 0);
    void getDate(int &den, int &sorokovnik, int &leto);

protected:
    void focusInEvent(QFocusEvent *);
    void focusOutEvent(QFocusEvent *);

private slots:
    void parseDate();
    void highlite(const QString &text);

private:
    QString mGreating;
    QMap<QString, int> mSorokovniki;

    int mDen;
    int mSorokovnik;
    int mLeto;
};

#endif // SOROKOVNIKLINEEDIT_H
