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

#ifndef DK4LETODB_H
#define DK4LETODB_H

#include "dk4leto.h"

class DK4LetoDB : public DK4Leto
{
    Q_OBJECT

public:
    DK4LetoDB();
    DK4LetoDB(int nomerLeta, int denNedeliNovoletiya, int krugLet,
              int stikhiyaICvet, int nachalo);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    DK4LetoDB *clone(int nomerLeta, int denNedeliNovoletiya, int krugLet,
                             int stikhiyaICvet, int nachalo);
};

#endif // DK4LETODB_H
