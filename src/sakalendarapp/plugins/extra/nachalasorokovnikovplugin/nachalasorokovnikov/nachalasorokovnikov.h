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

#ifndef NOVOLETIYA_H
#define NOVOLETIYA_H

#include <QGraphicsWidget>
#include <QButtonGroup>

class QGraphicsGridLayout;

class NachalaSorokovnikov : public QGraphicsWidget
{
    Q_OBJECT

public:
    NachalaSorokovnikov(QGraphicsItem *parent = 0);

    void vzyatInfo(int &mesyac, int &nachDen, bool &svyashchLeto);
    void ustanovitNovoletie(int novoletie);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

public slots:
    void vyibratNovoletie(int denNedeliNovoletiya);

signals:
    void vyibranoNovoletie(int mesyac, int pervDen, bool svyashch);

private slots:
    void test(int, int, bool);

private:
    QGraphicsLayout *shemaNachalaSorokovnikov();
    QGraphicsWidget *dobavitDenNedeli(QSizeF size, QString den, int nom,
                                      int mesyac, bool svyashch = false);

    QButtonGroup mDniNachMesButtonGroup;

    int mNachSorok;
};

#endif // NOVOLETIYA_H
