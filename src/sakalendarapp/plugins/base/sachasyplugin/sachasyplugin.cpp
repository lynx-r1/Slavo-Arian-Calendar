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

#include <QHBoxLayout>

#include "sachasyokno.h"

#include "sachasyplugin.h"

ChasyPlugin::ChasyPlugin()
{
}

QStringList ChasyPlugin::chasy() const
{
    return QStringList() << tr("Славяно-Арийские Часы");
}

QGraphicsWidget *ChasyPlugin::sozdatGraphicsChasy(QString chasy, QState *group,
                                                  QStateMachine *machine)
{
    if (chasy == tr("Славяно-Арийские Часы")) {
        SAChasyOkno *c = new SAChasyOkno(group, machine);
        return c;
    }
    return 0;
}

Q_EXPORT_PLUGIN2(chasyplugin, ChasyPlugin)
