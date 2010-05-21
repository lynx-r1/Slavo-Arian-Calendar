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

#include <QGraphicsView>
#include <QStringList>

#include "dk4okno.h"

#include "dk4plugin.h"

DK4Plugin::DK4Plugin()
{
}

QStringList DK4Plugin::krugolet() const
{
    return QStringList() << tr("Даарийский Круголет Числобога");
}

QGraphicsWidget *DK4Plugin::sozdatKrugolet(QString krugolet, QState *group, QStateMachine *machine)
{
    if (krugolet == tr("Даарийский Круголет Числобога")) {
        DK4Okno *k = new DK4Okno(group, machine);
        return k;
    }

    return 0;
}

Q_EXPORT_PLUGIN2(dk4plugin, DK4Plugin)
